namespace cpp thrift_test_kernal

const i32 MAX = 3;

enum ServiceType {
	Client = 0;
	Server = 1;
}
struct GetServiceListRequest {
	1: required ServiceType type;
}

struct GetServiceListResponse {
	1: required i32 errorCode;					//0: succ, 1: kernal internal error
	2: required list<string> instanceList;
}

struct AddServiceRequest {
	1: required ServiceType type;
	2: required string serviceName;
	3: required string version;
	4: required i32 port;
	5: required string transport;
	6: required string protocol;
	7: required string idlAbsFileName;
}

struct AddServiceResponse  {
	1: required i32 errorCode;					//0: succ, 1: kernal internal error
}

struct FillDataRequest {
	1: required ServiceType type;
	2: required string serviceName;
	3: required string methodName;
	4: required string data;
}

struct FillDataResponse {
	1: required i32 errorCode;					//0: succ, 1: kernal internal error
}

struct RequestTriggerRequest {
	1: required string serviceName;
	2: required string methodName;
}

struct RequestTriggerResponse {
	1: required i32 errorCode;					//0: succ, 1: kernal internal error
	2: required string reqponseJson;
}

service ThriftTestKernalService {
	GetServiceListResponse GetServiceList (1: GetServiceListRequest request);
	AddServiceResponse AddService (1:AddServiceRequest request);
	FillDataResponse FillData (1:FillDataRequest request);
	RequestTriggerResponse RequestTrigger (1:RequestTriggerRequest request);
}
