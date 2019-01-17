#ifndef SEND_H
#define SEND_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

// Class for sending messages
// mimics the send_lambda from https://github.com/boostorg/beast/blob/develop/example/http/server/sync-ssl/http_server_sync_ssl.cpp#L208
// But I believe moving it to a seperate class can help

namespace HTTPServer {

	template<class Stream>
	class Sender {
	public:
		//Sender(Stream& out_stream, bool& close, beast::error_code ec);
		Sender(Stream& out_stream, 
								bool& close, 
								beast::error_code ec){
			this->out_stream = out_stream;
			this->close = close;
			this->ec = ec;
		}

		// template<bool isRequest, class Body>
		// void send(http::message<isRequest, Body>& msg) const;
		template<bool isRequest, class Body>
		void send(http::message<isRequest, Body>& msg) const {
			close = msg.need_eof();

			// constify for write
			http::serializer<isRequest, Body> serial_msg{msg};
			http::write(out_stream, serial_msg, ec);
		}

	private:
		// Output stream for writing response
		Stream out_stream;

		// Connection closed semantic bool
		// true when should close connection after sending
		bool& close;

		// shared error code
		beast::error_code& ec;
	};

} // namespace HTTPServer


// CAnnot link with this file
// try not having an object file and have it recompile each time as needed
// compiler will ignore duplicate code but will tkae longer to compile

// if functions are defined in .cpp compiler will not know
// what versions of the templated function to compile
// having it in the header allows multple compilations



// needed for compilation
// template class HTTPServer::Sender<ssl::stream<tcp::socket>>;
// extern HTTPServer::Sender<ssl::stream<tcp::socket&>> _secure_sender;



#endif // SEND_H