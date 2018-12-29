#include "send.h"


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