#include "server.h"

#include "endpoints/example.h"

HTTPServer::HTTPServer(char * addr_, char * port_){
	addr = boost::asio::ip::make_address(addr_);
	port = static_cast<unsigned short>(std::atoi(port_));

	// TODO: figure out best way to initialize our endpoints
	//		 Most important feature is to have it initialized so
	// 		 we don't have to worry about thread-safety
	handlers.emplace("example", std::make_unique<endpoint::Example>());
	// handlers["example"]->handle("GET");
	// handlers["example"]->handle("POST");

	
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