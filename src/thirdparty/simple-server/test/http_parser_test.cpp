/*
 * http_parser_test.cpp
 *
 *  Created on: Oct 30, 2014
 *      Author: liao
 */
#include "http_parser.h"
#include "simple_log.h"

int main () {
	log_init("conf", "simple_log.conf");
	std::string temp = "  aaa:bbb\r\n";
	std::vector<std::string> parts;
	split_str(parts, temp, ':');
	log_error("parts[0]:%s", parts[0].c_str());
	log_debug("parts[1]:%s", parts[1].c_str());
	return 0;
}
