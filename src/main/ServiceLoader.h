#ifndef _service_loader_h_
#define _service_loader_h_

#include <pthread.h>
#include <string>
#include <thrift/server/TSimpleServer.h>

#include "ThriftTestKernelService.h"
#include "thrift_test_kernel_types.h"
#include "typedef.h"

using namespace std;
using namespace thrift_test_kernel;
using namespace apache::thrift::server;

class ServiceLoader {
private:
	pthread_mutex_t mutex;
	
	vector<string> compiledClientServiceId;
	vector<string> compiledServerServiceId;

	//map<serviceId, dlopen_return_value>
	map<string, void*> linkedLibClient;
	map<string, void*> linkedLibServer;
public:
	ServiceLoader();

public:
	int compile(string serviceId, string idlPath, ServiceType::type type);	
	int link(string serviceId, ServiceType::type type);	
	TSimpleServer* getServer(string serviceId);
	Client_Entry getMethod(string serviceId, string method);
};

#endif
