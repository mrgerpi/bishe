#include "TestService.h"

#include "simple_log.h"
#include <boost/shared_ptr.hpp>
#include <stdio.h>

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TSimpleFileTransport.h>
#include <thrift/transport/TTransport.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/TToString.h>

using namespace std;
using namespace ::test_service;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;


extern "C" int test_service_TestInterface_9838(string ip, int port)
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	socket->setConnTimeout(500);
	socket->setRecvTimeout(300);
	socket->setSendTimeout(300);


	shared_ptr<TestServiceClient> client(new TestServiceClient(protocol));
	using apache::thrift::to_string;

	TestServiceResponse rsp;

	TestServiceRequest req0;
	boost::shared_ptr<TTransport> req0_itrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/test_service/TestInterface/req0.json", true, true));
	boost::shared_ptr<TProtocol> req0_iprot(new TJSONProtocol(req0_itrans));
	req0.read(req0_iprot.get());
	log_info("test_service::TestInterface||entry||TestServiceRequest=%s", to_string(req0).c_str());	

	transport->open();
	try {
		client->TestInterface(rsp, req0);	
	} catch (TException& tx) {
		log_error("test_service::TestInterface Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("test_service::TestInterface ||rpc||rsp=%s", to_string(rsp).c_str());


	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/test_service/TestInterface/rsp.json", true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	FILE* fp = fopen("/home/Shit/thrift-test/src/../data/test_service/TestInterface/rsp.json", "w");
	fclose(fp);
	rsp.write(oprot.get());
	log_info("test_service::TestInterface ||exit||TestServiceResponse=%s", to_string(rsp).c_str());	
	return 0;
}
