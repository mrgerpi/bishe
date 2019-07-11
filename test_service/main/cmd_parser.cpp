#include "cmd_parser.h"
#include <stdio.h>

CmdParser::CmdParser(int argc, char** argv) 
{
	this->argc = argc;
	this->argv = argv;
	this->index = 0;
}

char* CmdParser::getNextPara() 
{
	char* p = NULL;
	if (index < argc) {
		p = argv[index];
		index++;
	}
	return p;
}
