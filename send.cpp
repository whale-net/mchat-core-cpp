#include "send.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>



namespace beast = boost::beast;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

namespace HTTPServer {
	template<class Stream>
	Sender<Stream>::Sender(Stream& out_stream, 
								bool& close, 
								beast::error_code ec){
		this->out_stream = out_stream;
		this->close = close;
		this->ec = ec;
	}

	template<class Stream>
	template<bool isRequest, class Body>
	void Sender<Stream>::send(http::message<isRequest, Body>& msg) const {
		close = msg.need_eof();

		// constify for write
		http::serializer<isRequest, Body> serial_msg{msg};
		http::write(out_stream, serial_msg, ec);
	}

	
}


// TODO: figure out a better way to do this perhaps
// namespace needed_for_compilation {
// 	boost::asio::io_context iocy{1};
// 	// ssl::stream<tcp::socket&> stream{sock, ssl_ctx};
// 	tcp::socket socky{iocy};
// 	ssl::context ssl_ctxy{ssl::context::sslv23};

// 	ssl::stream<tcp::socket&> streamy{socky, ssl_ctxy};
// 	bool closey;
// 	beast::error_code ecy;
// 	// TODO
// 	// uncomment this and fight the error, move sender to server.cpp?
// 	// HTTPServer::Sender<ssl::stream<tcp::socket&>> _secure_sender{streamy, closey, ecy};
// }