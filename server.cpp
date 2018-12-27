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

void HTTPServer::setup(char * addr_, char * port_){
	addr = boost::asio::ip::make_address(addr_);
	port = static_cast<unsigned short>(std::atoi(port_));

	// TODO: figure out best way to initialize our endpoints
	//		 Most important feature is to have it initialized so
	// 		 we don't have to worry about thread-safety
	handlers.emplace("example", std::make_unique<endpoint::Example>());
	// handlers["example"]->handle("GET");
	// handlers["example"]->handle("POST");

	
}

void HTTPServer::route(tcp::socket& sock){
	handlers["example"]->handle("GET");
	handlers["example"]->handle("POST");


	sock.shutdown(tcp::socket::shutdown_send);
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
			&HTTPServer::route,
			std::move(socket)
		)}.detach();
	}
}
