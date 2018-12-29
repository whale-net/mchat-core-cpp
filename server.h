#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>
#include <memory>
#include <string>


#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>

#include "endpoints/endpoint.h"

namespace beast = boost::beast; 		// from <boost/beast.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace http = beast::http;    // from <boost/beast/http.hpp>
namespace ssl = boost::asio::ssl;		// from <boos/asio/ssl.hpp>


// Generic http_server
namespace HTTPServer {

	// Acts as the 'constructor'
	void setup(char * addr_, char * port_);

	// Start serving requests
	void run();

	// Prepare request to be routed
	void prepare_request(tcp::socket& sock, ssl::context& ssl_ctx);

	// route request to proper handler
	void route(http::request<http::string_body>&& req);

	// generic cerr logging tool
	void log_error(beast::error_code ec, char const* what);


	// Mapping of api endpoint to a function
	// the /api/version<num>/ is implicit and not stored
	extern std::unordered_map<std::string, std::unique_ptr<API_Endpoint>> handlers;


	// Address from which the server will run
	extern boost::asio::ip::address addr;

	// Port from which the server will listen
	extern unsigned short port;

}

#endif // SERVER_H