package controllers

<<<<<<< HEAD
type UserLoginReq struct {
	Email    string `json:"email"`
	Password string `json:"password"`
}

type HttpRsp struct {
	ErrCode int         `json:"errCode"`
	ErrMsg  string      `json:"errMsg"`
	Data    interface{} `json:"data"`
}

type Service struct {
	ServiceName   string `json:"serviceName"`
	Port          int64  `json:"port"`
	Transport     string `json:"transport"`
	BuffTransport string `json:"buffTransport"`
	Protocol      string `json:"protocol"`
	Idl           string `json:"idl"`
	Ip            string `json:"ip"`
}

type AddProjectReq struct {
	Client  Service   `json:"client"`
	Servers []Service `json:"servers"`
}

type AddProjectRsp struct {
	SpId int64 `json:"servicePattermId"`
}

type Project struct {
	SpId    int64     `json:"servicePattermId"`
	Client  Service   `json:"client"`
	Servers []Service `json:"servers"`
}

type Method struct {
	ServiceName string `json:"serviceName"`
	MethodName  string `json:"methodName"`
}

type AddInterfaceReq struct {
	SpId       int64    `json:"servicePattermId"`
	Interface  string   `json:"interface"`
	Dependency []Method `json:"dependency"`
}

type Filed struct {
	FiledName string `json:"filedName"`
	FiledType string `json:"filedType"`
}

type StructDesc struct {
	ServiceName   string  `json:"serviceName"`
	InterfaceName string  `json:"interfaceName"`
	StructName    string  `json:"structName"`
	Fileds        []Filed `json:"fileds"`
}

type AddInterfaceRsp struct {
	IpId int64        `json:"interfacePattermId"`
	Reqs []StructDesc `json:"reqs"`
	Rsps []StructDesc `json:"rsps"`
}

type QueryInterfaceReq struct {
	SpId int64 `json:"servicePattermId"`
}

type StructData struct {
	ServiceName   string `json:"serviceName"`
	InterfaceName string `json:"interfaceName"`
	StructName    string `json:"structName"`
	Data          string `json:"data"`
}

type AddTestCaseReq struct {
	IpId int64        `json:"interfacePattermId"`
	Reqs []StructData `json:"reqs"`
	Rsps []StructData `json:"rsps"`
}

type AddTestCaseRsp struct {
	TcId int64 `json:"testcaseId"`
}

type QueryTestCaseReq struct {
	IpId int64 `json:"interfacePattermId"`
}

type TestCase struct {
	TcId int64        `json:"testcaseId"`
	Reqs []StructData `json:"reqs"`
	Rsps []StructData `json:"rsps"`
}

type QueryTestCaseRsp struct {
	TestCases []TestCase `json:"testcases"`
}

type TriggerReq struct {
	TcId int64 `json:"testcaseId"`
}

type TriggerRsp struct {
	Rsp string `json:"rsp"`
}
=======
type UserReq struct {
	Email    string `json:"email"`
	Password string `json:"password"`
}
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
