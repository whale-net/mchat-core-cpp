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
class HTTPServer {
public:
	HTTPServer(char * addr_, char * port_);

	// Start serving requests
	void run();

private:
	//NOTE: these are moved into run()
	// // io context needed for everything boost/networking
	// boost::asio::io_context ioc;

	// // acceptor that will listen for new requests
	// tcp::acceptor acceptor;

	// Mapping of api endpoint to a function
	// the /api/version<num>/ is implicit and not stored
	static std::unordered_map<std::string, std::unique_ptr<API_Endpoint>> handlers;


	// Address from which the server will run
	boost::asio::ip::address addr;

	// Port from which the server will listen
	unsigned short port;

};

#endif // SERVER_H