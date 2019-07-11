#include "ServiceSrcGenerator.h"
#include "didiutils.h"
#include "simple_log.h"

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <errno.h>
#include <vector>

using namespace std;

ServiceSrcGenerator::ServiceSrcGenerator()
{
}

int isDigit(string str, int& ret)
{
	istringstream iss(str);
	iss >> ret;
	if (iss.good()) {
		return 1;
	} else {
		return 0;
	}
}

int ServiceSrcGenerator::executeShell(string cmd, string& result)
{
	string realCmd = cmd + " 2>&1";
	FILE* fp = popen(realCmd.c_str(), "r");
	if (fp == NULL) {
		log_error("ServiceSrcGenerator::executeShell||popen fail||cmd=%s", realCmd.c_str());
		return 1;
	}

	char buff[256];		
	int ret = 0;
	do {
		if (fgets(buff, sizeof(buff), fp) == NULL) {
			break;		//EOF && no char be read
		}
		string line(buff);
		if (isDigit(line, ret)) {
			break;
		}
		result += line;
	}while(true);

	//errno == ECHILD: child process recover by wait system call
	if (pclose(fp) == -1 && errno != ECHILD) {
		log_error("ServiceSrcGenerator::executeShell||pclose fail||cmd=%s||err=%s", realCmd.c_str(), strerror(errno));
		return 1;
	}
	return ret;
}

int ServiceSrcGenerator::originalSrcGen(string serviceId, string idlPath) 
{
	vector<string> paras;
	DidiUtils::split_str(serviceId, paras, "&");	
	string cmd = DidiUtils::pwd() + "/script/originalSrcGen.sh " + idlPath + " " + paras[0];	
	log_info("ServiceSrcGenerator::originalSrcGen||start original gen script||cmd=%s",
			cmd.c_str());

	string result;
	int ret = executeShell(cmd, result);
	if (ret != 0) {
		log_error("ServiceSrcGenerator::originalSrcGen||orginal src gen script exe fail||result=%s||ret=%d", result.c_str(), ret);
		return ret;
	} else {
		log_info("ServiceSrcGenerator::originalSrcGen||orginal src gen script exe succ||result=%s", result.c_str());
		return 0;
	}
}

int ServiceSrcGenerator::clientSrcGen(string serviceId, string idlPath, string& method)
{
	vector<string> paras;
	DidiUtils::split_str(serviceId, paras, "&");	
	string cmd = DidiUtils::pwd() + "/script/clientSrcGen.sh " + idlPath 
		+ " " + paras[0] + " " + paras[2];
	log_info("ServiceSrcGenerator::clientSrcGen||start client src gen script||cmd=%s",
			cmd.c_str());

	int ret = ServiceSrcGenerator::executeShell(cmd, method);
	if (ret != 0) {
		log_error("ServiceSrcGenerator::clientSrcGen||client src gen script exe fail||result=%s||ret=%d", method.c_str(), ret);
		return ret;
	} else {
		log_info("ServiceSrcGenerator::clientSrcGen||client src gen script exe succ||result=%s", method.c_str());
		return 0;
	}
}

int ServiceSrcGenerator::serverSrcGen(string serviceId, string idlPath) 
{
	vector<string> paras;
	DidiUtils::split_str(serviceId, paras, "&");	
	string cmd = DidiUtils::pwd() + "/script/serverSrcGen.sh " + idlPath + " " + paras[0];
	log_info("ServiceSrcGenerator::serverSrcGen||start server src gen script||cmd=%s",
			cmd.c_str());

	string result;
	int ret = ServiceSrcGenerator::executeShell(cmd, result);
	if (ret != 0) {
		log_error("ServiceSrcGenerator::serverSrcGen||server src gen script exe fail||result=%s||ret=%d", result.c_str(), ret);
		return ret;
	} else {
		log_info("ServiceSrcGenerator::serverSrcGen||server src gen script exe succ||result=%s", result.c_str());
		return 0;
	}
}
