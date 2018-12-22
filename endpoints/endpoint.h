#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <string>

#include <boost/core/noncopyable.hpp>

// Class for handling http requests
class API_Endpoint : boost::noncopyable {
public:
	// Redirects to appropriate function depending on response
	// NOTE: need {} otherwise undefined reference compiler errors
	virtual void handle(std::string request){};
};

#endif // ENDPOINT_H