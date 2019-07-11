package kernel_client

import (
	"cgi/dao"
	rpc "cgi/kernel_client/gen-go/thrift_test_kernel"
	"errors"
	"fmt"
	"git.apache.org/thrift.git/lib/go/thrift"
	"github.com/astaxie/beego/logs"
)

func newKernelClient() (*rpc.ThriftTestKernalServiceClient, error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	protocolFactory := thrift.NewTBinaryProtocolFactoryDefault()
	transportFactory := thrift.NewTFramedTransportFactory(thrift.NewTTransportFactory())

	var transport thrift.TTransport
	transport, err := thrift.NewTSocket("127.0.0.1:9537")
	if err != nil {
		log.Error("newKernelClient||new socket error||err=%s", err.Error())
		return nil, err
	}

	transport = transportFactory.GetTransport(transport)

	client := rpc.NewThriftTestKernalServiceClientFactory(transport, protocolFactory)
	return client, nil
}

func reqAdapter(src *dao.Service, req_type int, req *rpc.AddServiceRequest) {
	if req_type == 0 {
		req.TypeA1 = rpc.ServiceType_Client
	} else if req_type == 1 {
		req.TypeA1 = rpc.ServiceType_Server
	}
	req.ServiceName = src.Name
	req.Version = src.Version
	req.Port = int32(src.Port)
	req.Transport = src.BuffTransport
	req.Protocol = src.Protocol
	req.IdlAbsFileName = src.Idl

	//ip
	if src.Ip != "" {
		req.Ip = &src.Ip
	}
}

func AddService(req_type int, service *dao.Service) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	req := rpc.NewAddServiceRequest()
	reqAdapter(service, req_type, req)

	client, err := newKernelClient()
	if err != nil {
		log.Error("kernel client||AddService||newKernelClient error||err=%s", err.Error())
		return err
	}
	//rpc call
	log.Info("kernle client||AddService||req=%+v", req)
	client.Transport.Open()
	rsp, err := client.AddService(req)
	client.Transport.Close()
	if err != nil {
		log.Error("kernel client||AddService||rpc error||err=%s", err.Error())
		return err
	}
	log.Info("kernle client||AddService||rsp=%+v", rsp)
	if rsp.GetErrorCode() != 0 {
		log.Error("kernel client||AddService||rpc ret error||ret=%d", rsp.GetErrorCode())
		return errors.New(fmt.Sprintf("kernel client ret=%d", rsp.GetErrorCode()))
	}
	return nil
}

func GetServiceList(req_type int) ([]string, error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	req := rpc.NewGetServiceListRequest()
	if req_type == 0 {
		req.TypeA1 = rpc.ServiceType_Client
	} else if req_type == 1 {
		req.TypeA1 = rpc.ServiceType_Server
	}

	client, err := newKernelClient()
	if err != nil {
		log.Error("kernel client||GetServiceList||newKernelClient error||err=%s", err.Error())
		return nil, err
	}
	//rpc call
	log.Info("kernle client||GetServiceList||req=%+v", req)
	client.Transport.Open()
	rsp, err := client.GetServiceList(req)
	client.Transport.Close()
	if err != nil {
		log.Error("kernel client||GetServiceList||rpc error||err=%s", err.Error())
		return nil, err
	}
	log.Info("kernle client||GetServiceList||rsp=%+v", rsp)
	if rsp.GetErrorCode() != 0 {
		log.Error("kernel client||GetServiceList||rpc ret error||ret=%d", rsp.GetErrorCode())
		return nil, errors.New(fmt.Sprintf("kernel client ret=%d", rsp.GetErrorCode()))
	}

	return rsp.GetInstanceList(), nil
}

func FillData(data_type int, serviceName string, methodName string, data string) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	req := rpc.NewFillDataRequest()
	if data_type == 0 {
		req.TypeA1 = rpc.ServiceType_Client
	} else if data_type == 1 {
		req.TypeA1 = rpc.ServiceType_Server
	}
	req.ServiceName = serviceName
	req.MethodName = methodName
	req.Data = data

	client, err := newKernelClient()
	if err != nil {
		log.Error("kernel client||filldata||newKernelClient error||err=%s", err.Error())
		return err
	}
	//rpc call
	log.Info("kernle client||filldata||req=%+v", req)
	client.Transport.Open()
	rsp, err := client.FillData(req)
	client.Transport.Close()
	if err != nil {
		log.Error("kernel client||filldata||rpc error||err=%s", err.Error())
		return err
	}
	log.Info("kernle client||filldata||rsp=%+v", rsp)
	if rsp.GetErrorCode() != 0 {
		log.Error("kernel client||filldata||rpc ret error||ret=%d", rsp.GetErrorCode())
		return errors.New(fmt.Sprintf("kernel client ret=%d", rsp.GetErrorCode()))
	}

	return nil
}

func RequestTrigger(serviceName string, methodName string) (ret string, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	req := rpc.NewRequestTriggerRequest()

	req.ServiceName = serviceName
	req.MethodName = methodName

	client, err := newKernelClient()
	if err != nil {
		log.Error("kernel client||RequestTrigger||newKernelClient error||err=%s", err.Error())
		return "", err
	}
	//rpc call
	log.Info("kernle client||RequestTrigger||req=%+v", req)
	client.Transport.Open()
	rsp, err := client.RequestTrigger(req)
	client.Transport.Close()
	if err != nil {
		log.Error("kernel client||RequestTrigger||rpc error||err=%s", err.Error())
		return "", err
	}
	log.Info("kernle client||RequestTrigger||rsp=%+v", rsp)
	if rsp.GetErrorCode() != 0 {
		log.Error("kernel client||RequestTrigger||rpc ret error||ret=%d", rsp.GetErrorCode())
		return "", errors.New(fmt.Sprintf("kernel client ret=%d", rsp.GetErrorCode()))
	}

	return rsp.GetReqponseJson(), nil
}
