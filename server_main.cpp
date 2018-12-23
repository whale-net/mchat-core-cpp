#include <iostream>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "server.h"

int main(int argc, char* argv[]){
	if (argc != 3){
		std::cerr << 
			"./server <address> <port>\n" <<
			"	./server 127.0.0.1 8000\n";
			return EXIT_FAILURE;
	}

	HTTPServer::setup(argv[1], argv[2]);
	HTTPServer::run();
}

