#include <iostream>
#include <thread>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "server.h"
#include "cert.h"
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

		route(std::move(req));

		if (ec) {
			return log_error(ec, "writing response");
		}
		// TODO:
		// handle Connection: close semantic

	}	

	//sock.shutdown(tcp::socket::shutdown_send);
	stream.shutdown(ec);
	if (ec) return log_error(ec, "shutdown");
}

// TODO template this, may be uncessary depeding on request type
void HTTPServer::route(http::request<http::string_body>&& req){

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
