#ifndef _service_src_generator_h_
#define _service_src_generator_h_

#include <string>

using namespace std;

/*
 * serviceId = serviceNmae_version_port_transport_protocol
 */

class ServiceSrcGenerator {
public:
	static int executeShell(string cmd, string& result);
public:
	ServiceSrcGenerator();

	/*
	 * @return
	 * 0: succ
	 * 1: compile error
	 * 2: internal error
	 */
	int originalSrcGen(string serviceId, string idlPath);
	int clientSrcGen(string serviceId, string idlPath, string& methodStr);
	int serverSrcGen(string serviceId, string idlPath);
};

#endif
