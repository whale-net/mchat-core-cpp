#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>
#include <memory>
#include <string>
#include <thread>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "endpoints/endpoint.h"

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>



// Generic http_server
namespace HTTPServer {

	// Acts as the 'constructor'
	void setup(char * addr_, char * port_);

	// Start serving requests
	void run();

	// Route requests (TODO? move to Endpoint namespace?)
	// where does it make the most sense to have?
	void route(tcp::socket& sock);


	//NOTE: these are moved into run()
	// // io context needed for everything boost/networking
	// boost::asio::io_context ioc;

	// // acceptor that will listen for new requests
	// tcp::acceptor acceptor;

	// Mapping of api endpoint to a function
	// the /api/version<num>/ is implicit and not stored
	extern std::unordered_map<std::string, std::unique_ptr<API_Endpoint>> handlers;


	// Address from which the server will run
	extern boost::asio::ip::address addr;

	// Port from which the server will listen
	extern unsigned short port;

}

#endif // SERVER_H