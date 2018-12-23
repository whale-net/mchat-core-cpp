#include "server.h"

#include "endpoints/example.h"

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
	handlers["example"]->handle("GET");
	handlers["example"]->handle("POST");

	
}

void HTTPServer::run(){
	boost::asio::io_context ioc{1};
	tcp::acceptor acceptor{ioc, {addr, port}};
	while (true){
		// block until new client request
		tcp::socket socket{ioc};
		acceptor.accept(socket);

		// std::thread(std::bind{
		// 	&do
		// })
	}
}
