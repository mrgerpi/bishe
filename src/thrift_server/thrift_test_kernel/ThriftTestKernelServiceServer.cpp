#include "ThriftTestKernelServiceServer.h"
#include "ThriftTestKernelServiceHandler.h"
#include "ThriftTestKernelService.h"
#include "didiutils.h"

#include "thrift/TProcessor.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/protocol/TProtocol.h"
#include "thrift/concurrency/ThreadManager.h"
#include "thrift/concurrency/PosixThreadFactory.h"
#include "simple_log.h" 


#define nThreadNum 5
#define nPort	9537
#define THREAD_STACK_SIZE  64


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace apache::thrift::concurrency;


ThriftTestKernelServiceServer* ThriftTestKernelServiceServer::p = NULL;

ThriftTestKernelServiceServer* ThriftTestKernelServiceServer::instance() {
	if (p == NULL) {
		p = new ThriftTestKernelServiceServer();
	} 
	return p;
}

int ThriftTestKernelServiceServer::startServer() {
	uint64_t begin = DidiUtils::timems();

	boost::shared_ptr<ThriftTestKernelServiceHandler> handler (new ThriftTestKernelServiceHandler());
	boost::shared_ptr<TProcessor> processor (new ThriftTestKernelServiceProcessor(handler));
	boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(nThreadNum);
	boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory>(
			new PosixThreadFactory(apache::thrift::concurrency::PosixThreadFactory::ROUND_ROBIN,
				apache::thrift::concurrency::PosixThreadFactory::NORMAL,
				THREAD_STACK_SIZE));
	threadManager->threadFactory(threadFactory);
	threadManager->start();
	manager_ = threadManager;

	server_ = new TNonblockingServer(processor, protocolFactory, nPort, threadManager);
	//server_->setServerEventHandler(boost::shared_ptr<TServerEventHandler>(new ClientIpServerEventHandler()));
<<<<<<< HEAD
	log_info ("star server t_cost: %lums, port=%d", DidiUtils::timems()- begin, nPort);
=======
	log_info ("star server t_cost: %lums", DidiUtils::timems()- begin);
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	try
	{
		server_->serve();
	}
	catch(TException& tx)
	{
		threadManager->stop();
		log_error("failed to start server:tx.what:%s",tx.what());
		exit(1);
	}
}

int ThriftTestKernelServiceServer::stopServer() {

}
