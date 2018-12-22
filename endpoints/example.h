#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "endpoint.h"

namespace endpoint{

class Example : public API_Endpoint {
public:
	void handle(std::string request);
private:
	// handle GET request for the Example endpoint
	void handle_GET();

	//handle POST request for the Example endpoint
	void handle_POST();
};

} // endpoint

#endif // EXAMPLE_H
