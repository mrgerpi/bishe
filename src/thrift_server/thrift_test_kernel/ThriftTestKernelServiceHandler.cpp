
//This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "ThriftTestKernelService.h"
#include "ThriftTestKernelServiceHandler.h"
#include "simple_log.h"
#include "ServiceManager.h"
#include "didiutils.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/TToString.h>
#include <string>
#include <vector>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::thrift_test_kernel;
using namespace  std;

ThriftTestKernelServiceHandler::ThriftTestKernelServiceHandler() 
{
	serviceMgr = new ServiceManager();
}

ThriftTestKernelServiceHandler::~ThriftTestKernelServiceHandler() 
{
	if (serviceMgr != NULL) {
		delete serviceMgr;
	}
}


void ThriftTestKernelServiceHandler::GetServiceList(GetServiceListResponse& _return, const GetServiceListRequest& request) {
	using apache::thrift::to_string;
<<<<<<< HEAD
	log_info("ThriftTestKernelServiceHandler::GetServiceList||entry||req=%s", to_string(request).c_str());
=======
	log_info("ThriftTestKernelServiceHandler::AddService||entry||req=%s", to_string(request).c_str());
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1

	vector<string> list;
	int ret = serviceMgr->getServiceList(request.type, list);
	_return.__set_errorCode(ret);
	if (ret == 0) {
		_return.__set_instanceList(list);
	}

<<<<<<< HEAD
	log_info("ThriftTestKernelServiceHandler::GetServiceList||exit||rsp=%s", to_string(_return).c_str());
=======
	log_info("ThriftTestKernelServiceHandler::AddService||exit||rsp=%s", to_string(_return).c_str());
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
}

void ThriftTestKernelServiceHandler::AddService(AddServiceResponse& _return, const AddServiceRequest& request) {
	using apache::thrift::to_string;
<<<<<<< HEAD
	log_info("ThriftTestKernelServiceHandler::GetServiceList||entry||req=%s", to_string(request).c_str());

	if (request.type == ServiceType::Client && request.__isset.ip == false) {
		log_error("ThriftTestKernelServiceHandler::GetServiceList||req.type=Client||req.__isset.ip == false");
=======
	log_info("ThriftTestKernelServiceHandler::AddService||entry||req=%s", to_string(request).c_str());

	if (request.type == ServiceType::Client && request.__isset.ip == false) {
		log_error("ThriftTestKernelServiceHandler::AddService||req.type=Client||req.__isset.ip == false");
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
		_return.__set_errorCode(2);
		return;
	}

	int ret = serviceMgr->addService(request);
	if (ret != 0) {
<<<<<<< HEAD
		log_error("ThriftTestKernelServiceHandler::GetServiceList||service manager add Service failed");
	}
	_return.__set_errorCode(ret);

	log_info("ThriftTestKernelServiceHandler::GetServiceList||exit||rsp=%s", to_string(_return).c_str());
=======
		log_error("ThriftTestKernelServiceHandler::AddService||service manager add Service failed");
	}
	_return.__set_errorCode(ret);

	log_info("ThriftTestKernelServiceHandler::AddService||exit||rsp=%s", to_string(_return).c_str());
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
}

void ThriftTestKernelServiceHandler::FillData(FillDataResponse& _return, const FillDataRequest& request) {
	using apache::thrift::to_string;
	log_info("ThriftTestKernelServiceHandler::FillData||entry||req=%s", to_string(request).c_str());

	vector<string> tokens;
	DidiUtils::split_str(request.serviceName, tokens, "&");
	string path = DidiUtils::pwd() + "/../data/" + tokens[0] + "/";
	if (request.type == ServiceType::Server) {
		path = path + request.methodName + "/rsp.json";	
	} else if (request.type == ServiceType::Client) {
		vector<string> tokens;
		DidiUtils::split_str(request.methodName, tokens, "#");
<<<<<<< HEAD
		path = path + tokens[0] + "/req" + tokens[1] + ".json";
=======
		path = path + tokens[1] + "/req" + tokens[0] + ".json";
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	}

	int ret = DidiUtils::writeFile(path, request.data);
	if (ret != 0) {
		log_error("ThriftTestKernelServiceHandler::FillData||fill data error||path=%s||data=%s||ret=%d", 
				path.c_str(), request.data.c_str(), ret);
		_return.__set_errorCode(ret);
	} else {
		log_info("ThriftTestKernelServiceHandler::FillData||fill data succ||path=%s||data=%s", 
				path.c_str(), request.data.c_str());
		_return.__set_errorCode(0);
	}
	
	log_info("ThriftTestKernelServiceHandler::FillData||exit||rsp=%s", to_string(_return).c_str());
}

void ThriftTestKernelServiceHandler::RequestTrigger(RequestTriggerResponse& _return, const RequestTriggerRequest& request) {
	using apache::thrift::to_string;
	log_info("ThriftTestKernelServiceHandler::RequestTrigger||entry||req=%s", to_string(request).c_str());
	
	string result;
	int ret = serviceMgr->requestTrigger(request.serviceName, request.methodName, result);
	if (ret != 0) {
		log_error("ThriftTestKernelServiceHandler::RequestTrigger||service manager request trigger failed");
		_return.__set_errorCode(ret);
		_return.__set_responseJson("");
	} else {
		log_info("ThriftTestKernelServiceHandler::RequestTrigger||request trigger succ||result=%s", result.c_str());
		_return.__set_errorCode(0);
		_return.__set_responseJson(result);
	}
	
	log_info("ThriftTestKernelServiceHandler::RequestTrigger||exit||rsp=%s", to_string(_return).c_str());
}

