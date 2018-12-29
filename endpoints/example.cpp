#include <iostream>

#include <boost/algorithm/string.hpp>

#include "example.h"

void endpoint::Example::handle(std::string request){
	if (boost::algorithm::equals(request, "POST"))
		handle_POST();
	else
		handle_GET();
}

void endpoint::Example::handle_GET(){
	std::cout << "Example GET" << std::endl;
}

void endpoint::Example::handle_POST(){
	std::cout << "Example POST" << std::endl;
}