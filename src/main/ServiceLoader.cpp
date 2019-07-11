#include "ServiceLoader.h"
#include "typedef.h"
#include "simple_log.h"
#include "didiutils.h"
#include "ServiceSrcGenerator.h"
#include "typedef.h"

#include <stdlib.h>
#include <pthread.h>
#include <dlfcn.h>
#include <algorithm>
#include <thrift/server/TSimpleServer.h>

using namespace thrift_test_kernel;
using namespace apache::thrift::server;

ServiceLoader::ServiceLoader() 
{
	pthread_mutex_init(&mutex, NULL);
}

int ServiceLoader::compile(string serviceId, string idlPath, ServiceType::type type)
{
	vector<string> paras;
	DidiUtils::split_str(serviceId, paras, "&");
	string typeStr;
	if (type == ServiceType::Client) {
		vector<string>::iterator it = find(compiledClientServiceId.begin(), 
				compiledClientServiceId.end(), serviceId);
		if (it != compiledClientServiceId.end()) {
			log_info("ServiceLoader::compile||client has been compiled||serviceId=%s", serviceId.c_str());
			return 0;
		} else {
			vector<string>::iterator it = find(compiledServerServiceId.begin(), 
					compiledServerServiceId.end(), serviceId);
			if (it == compiledServerServiceId.end()) {
				typeStr = " 1";					//Client
			} else {
				log_info("ServiceLoader::compile||server has been compiled||serviceId=%s", serviceId.c_str());
				typeStr = " 3";					//Only_Client
			}
		}
	} else if (type == ServiceType::Server) {
		vector<string>::iterator it = find(compiledServerServiceId.begin(), 
				compiledServerServiceId.end(), serviceId);
		if (it != compiledServerServiceId.end()) {
			log_info("ServiceLoader::compile||server has been compiled||serviceId=%s", serviceId.c_str());
			return 0;
		} else {
			vector<string>::iterator it = find(compiledClientServiceId.begin(), 
					compiledClientServiceId.end(), serviceId);
			if (it == compiledClientServiceId.end()) {
				typeStr = " 0";					//Server
			} else {
				log_info("ServiceLoader::compile||client has been compiled||serviceId=%s", serviceId.c_str());
				typeStr = " 2";					//Only_Server
			}
		}
	}

	string cmd = DidiUtils::pwd() + "/script/compile.sh " + idlPath + " " + paras[0] + typeStr;
	
	log_info("ServiceLoader::compile||cmd=%s", cmd.c_str());	

	string result;
	int ret = ServiceSrcGenerator::executeShell(cmd, result);
	if (ret != 0) {
		log_error("ServiceLoader::compile||compile script exe fail||result=%s||ret=%d", result.c_str(), ret);
		return ret;
	} else {
		log_info("ServiceLoader::compile||compile script exe succ||result=%s", result.c_str());
	}
	
	if (type == ServiceType::Client) {
		pthread_mutex_lock(&mutex);
		compiledClientServiceId.push_back(serviceId);
		pthread_mutex_unlock(&mutex);
	} else if (type == ServiceType::Server) {
		pthread_mutex_lock(&mutex);
		compiledServerServiceId.push_back(serviceId);
		pthread_mutex_unlock(&mutex);
	}
	return 0;
}


int ServiceLoader::link(string serviceId, ServiceType::type type)
{
	vector<string> paras;
	DidiUtils::split_str(serviceId, paras, "&");
	string libPath = DidiUtils::pwd() + "/service_lib/";
	if (type == ServiceType::Client) {
		libPath = libPath + "lib" + paras[0] + "client.so";
<<<<<<< HEAD

		map<string, void*>::iterator it = linkedLibClient.find(serviceId);
		if (it != linkedLibClient.end()) {
			log_info("ServiceLoader::link||client lib linked already||lib=%s", libPath.c_str());
			return 0;
		}
	} else if (type == ServiceType::Server) {
		libPath = libPath + "lib" + paras[0] + "server.so";

		map<string, void*>::iterator it = linkedLibServer.find(serviceId);
		if (it != linkedLibServer.end()) {
			log_info("ServiceLoader::link||server lib linked already||lib=%s", libPath.c_str());
			return 0;
		}
=======
	} else if (type == ServiceType::Server) {
		libPath = libPath + "lib" + paras[0] + "server.so";
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	}

	dlerror();
	void* p = dlopen(libPath.c_str(), RTLD_LAZY);
	if (p == NULL) {
		log_error("ServiceLoader::link||fail to link service lib||lib=%s||err=%s", 
				libPath.c_str(), dlerror());
		return 1;
	}

	if (type == ServiceType::Client) {
		pthread_mutex_lock(&mutex);
		linkedLibClient[serviceId] = p;
		pthread_mutex_unlock(&mutex);
	} else if (type == ServiceType::Server) {
		pthread_mutex_lock(&mutex);
		linkedLibServer[serviceId] = p;
		pthread_mutex_unlock(&mutex);
	}

	log_info("ServiceLoader::link||link service lib succ||lib=%s", libPath.c_str());
	return 0;
}

TSimpleServer* ServiceLoader::getServer(string serviceId)
{
	vector<string> paras;
	DidiUtils::split_str(serviceId, paras, "&");
	string serviceName = paras[0];
	int port = atoi(paras[2].c_str());
	string serverEntryStr = "get_" + paras[0] + "_server";

	void* handler;
	map<string, void*>::iterator it = linkedLibServer.find(serviceId);
	if (it == linkedLibServer.end()) {
		log_error("ServiceLoader::getServer||server lib not linked||serviceId=%s", 
				serviceId.c_str());
		return NULL;
	} else {
		handler = it->second;
	}

	dlerror();
	Server_Entry entry = (Server_Entry)dlsym(handler, serverEntryStr.c_str());
	char* err = dlerror();	
	if (err != NULL) {
		log_error("ServiceLoader::getServer||fail to find synbol %s||serviceId=%s||err=%s", 
				serverEntryStr.c_str(), serviceId.c_str(), err);
		return NULL;
	}

	return entry(port);
}

Client_Entry ServiceLoader::getMethod(string serviceId, string method) 
{
	vector<string> paras;
	DidiUtils::split_str(serviceId, paras, "&");
	string serviceName = paras[0];
	string port = paras[2];
	string entryStr = serviceName + "_"  + method + "_" + port;
	
	void* handler;
	map<string, void*>::iterator it = linkedLibClient.find(serviceId);
	if (it == linkedLibClient.end()) {
		log_error("ServiceLoader::getServer||client lib not linked||serviceId=%s", 
				serviceId.c_str());
		return NULL;
	} else {
		handler = it->second;
	}

	dlerror();
	Client_Entry entry = (Client_Entry)dlsym(handler, entryStr.c_str());
	char* err = dlerror();	
	if (err != NULL) {
		log_error("ServiceLoader::getServer||fail to find synbol %s||serviceId=%s||err=%s", 
				entryStr.c_str(), serviceId.c_str(), err);
		return NULL;
	}

	return entry;
}


