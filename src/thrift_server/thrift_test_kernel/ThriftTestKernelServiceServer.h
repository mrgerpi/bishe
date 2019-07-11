#ifndef _thrift_test_kernel_server_h_
#define _thrift_test_kernel_server_h_

#include "thrift/server/TNonblockingServer.h"

class ThriftTestKernelServiceServer {
	private:
		ThriftTestKernelServiceServer() :server_(NULL) {}
		~ThriftTestKernelServiceServer() 
		{
			if (server_ != NULL) {
				delete server_;
				server_ = NULL;
			} 
		}
		static ThriftTestKernelServiceServer* p;
		::apache::thrift::server::TNonblockingServer* server_;
		boost::shared_ptr<apache::thrift::concurrency::ThreadManager> manager_;
	public:
		static ThriftTestKernelServiceServer* instance();
		int startServer();
		int stopServer();
};

#endif
