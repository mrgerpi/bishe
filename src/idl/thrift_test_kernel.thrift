namespace cpp thrift_test_kernel

const i32 MAX = 3;

enum ServiceType {
	Client = 0;
	Server = 1;
}
struct GetServiceListRequest {
<<<<<<< HEAD
1: required ServiceType type;		//0: Client, 1: Server
=======
	1: required ServiceType type;
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
}

struct GetServiceListResponse {
	1: required i32 errorCode;					//0: succ, 1: kernel internal error
<<<<<<< HEAD
	2: required list<string> instanceList;
}

=======
	2: required list<string> instanceList;		//serviceId list
}

//serviceId: serviceName_version_port_transport_protocol

>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
struct AddServiceRequest {
	1: required ServiceType type;
	2: required string serviceName;
	3: required string version;
	4: required i32 port;
	5: required string transport;
	6: required string protocol;
	7: required string idlAbsFileName;
	8: optional string ip;
}

struct AddServiceResponse  {
	1: required i32 errorCode;					//0: succ, 1: kernel internal error
}

struct FillDataRequest {
	1: required ServiceType type;
	2: required string serviceName;
<<<<<<< HEAD
	3: required string methodName;
=======
	3: required string methodName;		//1#methodName
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	4: required string data;
}

struct FillDataResponse {
	1: required i32 errorCode;					//0: succ, 1: kernel internal error
}

struct RequestTriggerRequest {
<<<<<<< HEAD
	1: required string serviceName;
=======
	1: required string serviceName;			//serviceId 
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	2: required string methodName;
}

struct RequestTriggerResponse {
	1: required i32 errorCode;					//0: succ, 1: kernel internal error
	2: required string responseJson;
}

service ThriftTestKernelService {
	GetServiceListResponse GetServiceList (1: GetServiceListRequest request);
	AddServiceResponse AddService (1:AddServiceRequest request);
	FillDataResponse FillData (1:FillDataRequest request);
	RequestTriggerResponse RequestTrigger (1:RequestTriggerRequest request);
}
