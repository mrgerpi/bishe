//This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "ThriftTestKernelService.h"
#include "ServiceManager.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::thrift_test_kernel;

class ThriftTestKernelServiceHandler : virtual public ThriftTestKernelServiceIf {
private:
	ServiceManager* serviceMgr;
public:
	ThriftTestKernelServiceHandler();

	~ThriftTestKernelServiceHandler();

	void GetServiceList(GetServiceListResponse& _return, const GetServiceListRequest& request);

	void AddService(AddServiceResponse& _return, const AddServiceRequest& request);

	void FillData(FillDataResponse& _return, const FillDataRequest& request);

	void RequestTrigger(RequestTriggerResponse& _return, const RequestTriggerRequest& request);
};
