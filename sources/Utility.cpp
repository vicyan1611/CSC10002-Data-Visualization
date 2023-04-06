#include <Utility.hpp>
#include <sstream>

int toInt(const std::string& str) {
		std::stringstream ss;
	ss << str;
	int i;
	ss >> i;
	return i;
}