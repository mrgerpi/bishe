#ifndef _cmd_parse_h_
#define _cmd_parse_h_

class CmdParser {
private:
	int index;
	int argc;
	char** argv;
public:
	CmdParser(int argc, char** argv);
	char* getNextPara();
};

#endif
