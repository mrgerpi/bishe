#ifndef _service_manager_h_
#define _service_manager_h_

#include "ThriftTestKernelService.h"
#include "ServiceSrcGenerator.h"
#include "ServiceLoader.h"
#include "typedef.h"

#include <pthread.h>
#include <thrift/server/TSimpleServer.h>
#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace thrift_test_kernel;
using namespace apache::thrift::server;

typedef struct Server_Args {
	Server_Args(string str, TSimpleServer* s) {
		serviceId = str;
		server = s;
	}
	string serviceId;
	TSimpleServer* server;
} Server_Args;

//serviceId format: serviceName_version_port_transport_protocol
class ServiceManager {
private:
	pthread_mutex_t mutex;
	ServiceSrcGenerator* generator;
	ServiceLoader* loader;

	map<string/* serviceId */, TSimpleServer*> serverMap;		
	map<string/* serviceId */, pthread_t> serverThreadMap;
	map<string/* serviceId */, map<string/* method */, Client_Entry>> methodMap;

	/*
	 *	对于Service 源码生成状态的维护可以下放到 ServiceSrcGenerator 
	 */
	vector<string> originalSrcServiceId;
	vector<string> serverSrcServiceId;
	vector<string> clientSrcServiceId;
private:
	string serviceIdGen(const AddServiceRequest& req);
public:
	ServiceManager();
	~ServiceManager();
public:
	/*
	 * @return
	 * 0:succ
	 * 1:kernel internal error
	 * 2:compile error, idl file invalid
	 * 3:para error
	 */
	int addService(const AddServiceRequest& req);

	int getServiceList(ServiceType::type type, vector<string>& list);

	int requestTrigger(string serviceName, string methodName, string& rsp);
};

#endif
