// thanks to my man vinnie falco for inspiring this function

#ifndef CERT_H
#define CERT_H


#include <boost/asio/ssl/context.hpp>

inline void load_cert(boost::asio::ssl::context& ctx){
	// openssl dhparam -out dh.pem 2048
	// openssl req -newkey rsa:2048 -nodes -keyout key.pem -x509 -days 10000 -out cert.pem
	ctx.use_certificate_chain_file("cert.pem");
	ctx.use_private_key_file("key.pem", boost::asio::ssl::context::pem);
	ctx.use_tmp_dh_file("dh.pem");
}


#endif // CERT_H