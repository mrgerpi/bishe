#include "simple_log.h"
#include "thrift_test_client.h"
#include "ThriftTestKernelService.h"

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TSimpleFileTransport.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <string>
#include <vector>

#define Gen_Input_Switch 0

using namespace std;
using namespace thrift_test_kernel;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift;

ThriftTestClient::ThriftTestClient(int port, string ip) 
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> ts(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(ts));

	socket->setConnTimeout(500);
//	socket->setRecvTimeout(1000 * 60);
//	socket->setSendTimeout(300);

	clientPtr = boost::make_shared<ThriftTestKernelServiceClient>(protocol);
	transport = clientPtr->getInputProtocol()->getTransport();
}

int sizeOfFile(string path) 
{
	string cmd = "cat " + path + " | wc -c";
	FILE* fp = popen(cmd.c_str(), "r");
	if (fp == NULL) {
		log_error("popen file failed, cmd:%s, error:%s", cmd.c_str(), strerror(errno));
		return -1;
	}
	char numStr[10];
	if (fgets(numStr, 10, fp) == NULL) {
		log_error("popen read file failed, path:%s, error:%s", path.c_str(), strerror(errno));
		return -1;
	}

	if (pclose(fp) == -1 && errno != ECHILD) {
		log_error("close file failed, error:%s", strerror(errno));
		return -1;
	}
	
	return atoi(numStr);
}

int ThriftTestClient::readAllFile(string path, string& content) 
{
	FILE* fp = fopen(path.c_str(), "r+");
	if (fp == NULL) {
		log_error("readAllFile open file failed, path:%s, error:%s", path.c_str(), strerror(errno));
		return -1;
	}

	int fileSize = sizeOfFile(path);
	if (fileSize == -1) 
		return -1;
	char* s = (char*)malloc(fileSize + 1);

	int i, ch;
	for(i = 0;(ch = fgetc(fp)) != EOF;i++) {
		s[i] = ch;
	}

<<<<<<< HEAD
	content.assign(s, fileSize);
=======
	content.assign(s, fileSize + 1);
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	free(s);

	if (fclose(fp) != 0) {
		log_error("writeAllFile close file failed, error:%s", strerror(errno));
		return -1;
	}

	return 0;
}

int ThriftTestClient::writeAllFile(const string& path, const string& content)
{
	FILE* fp = fopen(path.c_str(), "w+");
	if (fp == NULL) {
		log_error("writeAllFile open file failed, path=%s, error:%s", path.c_str(), strerror(errno));
		return -1;
	}

	int ret = fputs(content.c_str(), fp);
	if (ret == EOF) {
		log_error("writeAllFile write file failed, path=%s, error:%s", path.c_str(),  strerror(errno));
		return -1;
	}

	if (fclose(fp) != 0) {
		log_error("writeAllFile close file failed, error:%s", strerror(errno));
		return -1;
	}
	return 0;
}

TProtocol* ThriftTestClient::getJsonProtocol(string path) 
{
	boost::shared_ptr<TSimpleFileTransport> transport(new TSimpleFileTransport(path, true, true));
	boost::shared_ptr<TJSONProtocol> prot(new TJSONProtocol(transport)); 
	TProtocol* iprot = (TProtocol*)prot.get();
	return iprot;
}

string getDataPath(const string& methond, int type)
{
	string res;
	if (type == REQ) {
		res = "/home/Shit/thrift-test/kernel_client/data/" + methond + "/req.json";
	} else if (type == RSP) {
		res = "/home/Shit/thrift-test/kernel_client/data/" + methond + "/rsp.json";
	}
	return res;
}

int ThriftTestClient::GetServiceList(const string& reqJson, string& rspJson, char* type)
{
	log_info("GetServiceList||entry||reqJson=%s", reqJson.c_str());
	using apache::thrift::to_string;
	int ret = 0;
	GetServiceListRequest req;
	GetServiceListResponse rsp; 

	/*
	//reqJson to jsonFile	./data/GetServiceList/req.json
	string reqPath = getDataPath("GetServiceList", REQ);
	if (writeAllFile(reqPath, reqJson) == -1) {
		return -1;
	}

	//jsonFile to req
	boost::shared_ptr<TSimpleFileTransport> inputFileTrans(new TSimpleFileTransport(reqPath, true, true));
	boost::shared_ptr<TJSONProtocol> iprot(new TJSONProtocol(inputFileTrans)); 
	TProtocol* in = (TProtocol*)iprot.get();
	req.read(in);
	*/

	req.__set_type((const ServiceType::type)atoi(type));
	log_info("GetServiceList||get req||req=%s", to_string(req).c_str());

	//client call
	
	transport->open();
	try {
		clientPtr->GetServiceList(rsp, req);
	} catch(TException& tx) {
		log_error("GetServiceList Exception, message = %s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("GetServiceList||rpc||rsp=%s", to_string(rsp).c_str());

	//rsp to jsonFile
	string rspPath = getDataPath("GetServiceList", RSP);
	remove(rspPath.c_str());
	boost::shared_ptr<TSimpleFileTransport> outputFileTrans(new TSimpleFileTransport(rspPath, true, true));
	boost::shared_ptr<TJSONProtocol> oprot(new TJSONProtocol(outputFileTrans)); 
	TProtocol* out = (TProtocol*)oprot.get();
	rsp.write(out);
	//jsonFile to rspJson
	if (readAllFile(rspPath, rspJson) == -1) {
		return -1;
	}
	log_info("GetServiceList||exit||rspJson=%s||rspPath=%s", rspJson.c_str(), rspPath.c_str());

	return 0;
}

int ThriftTestClient::AddService(const string& reqJson, string& rspJson)
{
	log_info("AddService||entry||reqJson=%s", reqJson.c_str());
	using apache::thrift::to_string;
	int ret = 0;
	FILE* fp;
	AddServiceRequest req;
	AddServiceResponse rsp; 

	if (Gen_Input_Switch) {		//create input json file
		req.__set_type(ServiceType::Client);
		req.__set_serviceName("testService");
		req.__set_version("v1.1.1");
		req.__set_port(1234);
		req.__set_transport("TFileTransport");
		req.__set_protocol("TJSONProtocol");
		req.__set_idlAbsFileName("./data/idl.thrift");

		string path = "/home/Shit/thrift-test/kernel_client/input/AddService.json";
		boost::shared_ptr<TTransport> t(new TSimpleFileTransport(path, true, true));
		boost::shared_ptr<TProtocol> p(new TJSONProtocol(t));
		fp = fopen(path.c_str(), "w+");
		fclose(fp);
		req.write(p.get());
		return 0;
	}

	string reqPath = getDataPath("AddService", REQ);
	if (writeAllFile(reqPath, reqJson) == -1) {
		return -1;
	}
	boost::shared_ptr<TTransport> itrans(new TSimpleFileTransport(reqPath, true, true));
	boost::shared_ptr<TProtocol> iproc(new TJSONProtocol(itrans));
	req.read(iproc.get());	
	log_info("AddService||read req||req=%s", to_string(req).c_str());

	transport->open();
	try {
		clientPtr->AddService(rsp, req);
	} catch (TException& tx) {
		log_error("AddServcie Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("AddService||rpc||rsp=%s", to_string(rsp).c_str());

	string rspPath = getDataPath("AddService", RSP);
	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport(rspPath, true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	fp = fopen(rspPath.c_str(), "w+");
	fclose(fp);
	rsp.write(oprot.get());

	if (readAllFile(rspPath, rspJson) == -1) {
		return -1;
	}

	log_info("AddService||write rsp||rspJson=%s", rspJson.c_str());
	return 0;
}

int ThriftTestClient::FillData(const string& reqJson, string& rspJson)
{
	log_info("FillData||entry||reqJson=%s", reqJson.c_str());
	using apache::thrift::to_string;
	int ret = 0;
	FILE* fp;
	FillDataRequest	req;
	FillDataResponse rsp;

	if (Gen_Input_Switch) {		//create input json file
		req.__set_type(ServiceType::Client);
		req.__set_serviceName("testService");
		req.__set_methodName("testMethod");
		req.__set_data("{\"data\":0}");

		string path = "/home/Shit/thrift-test/kernel_client/input/FillData.json";
		boost::shared_ptr<TTransport> t(new TSimpleFileTransport(path, true, true));
		boost::shared_ptr<TProtocol> p(new TJSONProtocol(t));
		fp = fopen(path.c_str(), "w+");
		fclose(fp);
		req.write(p.get());
		return 0;
	}

	string reqPath = getDataPath("FillData", REQ);
	if (writeAllFile(reqPath, reqJson) == -1) {
		return -1;
	}
	boost::shared_ptr<TTransport> itrans(new TSimpleFileTransport(reqPath, true, true));
	boost::shared_ptr<TProtocol> iproc(new TJSONProtocol(itrans));
	req.read(iproc.get());	
	log_info("FillData||read req||req=%s", to_string(req).c_str());

	transport->open();
	try {
		clientPtr->FillData(rsp, req);
	} catch (TException& tx) {
		log_error("FillData Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("FillData||rpc||rsp=%s", to_string(rsp).c_str());

	string rspPath = getDataPath("FillData", RSP);
	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport(rspPath, true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	fp = fopen(rspPath.c_str(), "w+");
	fclose(fp);
	rsp.write(oprot.get());

	if (readAllFile(rspPath, rspJson) == -1) {
		return -1;
	}

	log_info("FillData||write rsp||rspJson=%s", rspJson.c_str());
	return 0;
}

int ThriftTestClient::RequestTrigger(const string& reqJson, string& rspJson)
{
	log_info("RequestTrigger||entry||reqJson=%s", reqJson.c_str());
	using apache::thrift::to_string;
	int ret = 0;
	FILE* fp;
	RequestTriggerResponse rsp; 
	RequestTriggerRequest req;

	if (Gen_Input_Switch) {		//create input json file
		req.__set_serviceName("testService");
		req.__set_methodName("testMethod");

		string path = "/home/Shit/thrift-test/kernel_client/input/RequestTrigger.json";
		boost::shared_ptr<TTransport> t(new TSimpleFileTransport(path, true, true));
		boost::shared_ptr<TProtocol> p(new TJSONProtocol(t));
		fp = fopen(path.c_str(), "w+");
		fclose(fp);
		req.write(p.get());
		return 0;
	}

	string reqPath = getDataPath("RequestTrigger", REQ);
	if (writeAllFile(reqPath, reqJson) == -1) {
		return -1;
	}
	boost::shared_ptr<TTransport> itrans(new TSimpleFileTransport(reqPath, true, true));
	boost::shared_ptr<TProtocol> iproc(new TJSONProtocol(itrans));
	req.read(iproc.get());	
	log_info("RequestTrigger||read req||req=%s", to_string(req).c_str());

	transport->open();
	try {
		clientPtr->RequestTrigger(rsp, req);
	} catch (TException& tx) {
		log_error("RequestTrigger Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("RequestTrigger||rpc||rsp=%s", to_string(rsp).c_str());

	string rspPath = getDataPath("RequestTrigger", RSP);
	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport(rspPath, true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	fp = fopen(rspPath.c_str(), "w+");
	fclose(fp);
	rsp.write(oprot.get());

	if (readAllFile(rspPath, rspJson) == -1) {
		return -1;
	}

	log_info("RequestTrigger||write rsp||rspJson=%s", rspJson.c_str());
	return 0;
}
