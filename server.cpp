#include <iostream>
#include <thread>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "server.h"
#include "cert.h"
#include "send.h"
#include "endpoints/example.h"


// Declare the variables used by our HTTPServer's functions
// This seems far better to do than having globals
namespace HTTPServer {
	// Mapping of api endpoint to a function
	// the /api/version<num>/ is implicit and not stored
	std::unordered_map<std::string, std::unique_ptr<API_Endpoint>> handlers;


	// Address from which the server will run
	boost::asio::ip::address addr;

	// Port from which the server will listen
	unsigned short port;
}

// https://github.com/boostorg/beast/blob/develop/example/http/server/sync-ssl/http_server_sync_ssl.cpp#L208
// will change to more robust logging later
void HTTPServer::log_error(beast::error_code ec, char const* what){
	std::cerr << what << ":" << ec.message() << "\n"; 	
}

void HTTPServer::setup(char * addr_, char * port_){
	addr = boost::asio::ip::make_address(addr_);
	port = static_cast<unsigned short>(std::atoi(port_));

	// TODO: figure out best way to initialize our endpoints
	//		 Most important feature is to have it initialized so
	// 		 we don't have to worry about thread-safety
	//		 
	//		 would it be possible to add them in hot? through a subprocess?
	//		 would not be useful but would be cool
	handlers.emplace("example", std::make_unique<endpoint::Example>());
	// handlers["example"]->handle("GET");
	// handlers["example"]->handle("POST");

	
}

// TODO figure out why it's not socket&&
// moving should be rvalue but lengthy compiler error
// unlike route
void HTTPServer::prepare_request(tcp::socket& sock, ssl::context& ssl_ctx){
	// begin parsing information about request
	// route to correct endpoint handler

	// Beast error code, !=0 -> error
	beast::error_code ec;

	// construct stream and perform handshake
	ssl::stream<tcp::socket&> stream{sock, ssl_ctx};
	stream.handshake(ssl::stream_base::server, ec);
	if (ec) return log_error(ec, "handshake");


	// handlers["example"]->handle("GET");
	// handlers["example"]->handle("POST");

	// loop and read into buffer
	beast::flat_buffer buffer;
	bool close = false;
	while (true){
		http::request<http::string_body> req;
		http::read(stream, buffer, req, ec);

		// exit condition
		if(ec == http::error::end_of_stream){
			break;
		}
		// otherwise bad read
		else if (ec) {
			return log_error(ec, "reading request");
		}
		// TODO: template this function
		route<http::string_body, ssl::stream<tcp::socket&>>(std::move(req), std::ref(stream), ec, close);

		if (ec) {
			return log_error(ec, "writing response");
		}
		// handle Connection: close semantic
		if (close){
			break;
		}
	}	

	//sock.shutdown(tcp::socket::shutdown_send);
	stream.shutdown(ec);
	if (ec) return log_error(ec, "shutdown");
}

// TODO template this, may be uncessary depeding on request type
template <class Body, class Stream>
void HTTPServer::route(http::request<Body>&& req,
						   Stream& stream,
						   beast::error_code& ec,
						   bool& close){
	// Determine if we should close socket after write
	// moved to Sender::send
	// close = req.need_eof();

	// creates a message
	auto const bad_request =
    [&req](beast::string_view why)
    {
        http::response<Body> res{http::status::bad_request, req.version()};
        // res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = why.to_string();
        res.prepare_payload();
        return res;
	};

	http::response<Body> sample_response = bad_request("it work");

	// this is better than what I have now but will still need work 
	// in order to generalize it further
	HTTPServer::Sender<Stream> sender{stream, close, ec};
	// sender.send(sample_response);


	// // because this needs to be templated our send function
	// // cannot be lambda
	// http::serializer<false, Body> sr{res}; //Note: cannot serialize in place
	// // this will be placed in handlers most likely
	// // have to consider best way to send
	// // could yoink send_lambda but I think there will be a better way
	// // inline function?


	// http::write(stream, sr, ec);
}

void HTTPServer::run(){
	boost::asio::io_context ioc{1};
	tcp::acceptor acceptor{ioc, {addr, port}};

	ssl::context ssl_ctx{ssl::context::sslv23};
	load_cert(ssl_ctx);

	while (true){
		// block until new client request
		tcp::socket socket{ioc};
		acceptor.accept(socket);

		// 
		// Spawn thread on handler
		// do as little work as possible
		std::thread{std::bind(
			&HTTPServer::prepare_request,
			std::move(socket),
			std::ref(ssl_ctx)
		)}.detach();
	}
}
