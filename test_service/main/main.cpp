#include <iostream>
#include <string>
#include <vector>
#include <signal.h>
#include <execinfo.h>
#include <pthread.h>

#include "simple_log.h"
#include "TestServiceServer.h"

using namespace std;

const static string log_conf_dir = "conf";
const static string log_conf_filename = "simple.conf";

void DumpTraceback(int Signal)
{
    const int len = 200;
    void* buffer[len];
    int nptrs = ::backtrace(buffer, len);		//
    char** strings = ::backtrace_symbols(buffer, nptrs);
    if (strings)
    {
      for (int i = 0; i < nptrs; ++i)
      {
		log_error("DumpTraceBack||line=%d||trace_back=%s", i, strings[i]);
      }
      free(strings);
    }
    exit(1);
}

int sig_actions() 
{
	/*
	// Delay exit
    signal(SIGTERM, DelayExit);
	*/
    // Ignore signal
    signal(SIGTERM, SIG_IGN);
	signal(SIGINT, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // Dump traceback when crash.
    // Core signal's default action is to terminate the process and dump core.
    signal(SIGBUS, DumpTraceback);  // 10 Core  Bus error (bad memory access)
    signal(SIGSEGV, DumpTraceback); // 11 Core  Invalid memory reference
    signal(SIGABRT, DumpTraceback); // 6  Core  Abort signal from abort(3)
    signal(SIGILL, DumpTraceback);  // 4  Core  Illegal Instruction
    signal(SIGFPE, DumpTraceback);  // 8  Core  Floating point exception
}


int main(int argc, char* argv[]) 
{
	log_init(log_conf_dir.c_str(), log_conf_filename.c_str());
	log_info("%s", "welcome to test service! :) ");
	sig_actions();

	start_server();
	return 0;
}
