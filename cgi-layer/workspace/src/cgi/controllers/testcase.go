package controllers

import (
	"cgi/dao"
	kclient "cgi/kernel_client"
	"encoding/json"
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/logs"
	"strconv"
	/*
		"github.com/astaxie/beego/orm"
		"os/exec"
		"strings"
	*/)

type TestCaseController struct {
	beego.Controller
}

func (this *TestCaseController) TestCaseEntry() {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	action := this.Ctx.Input.Param(":splat")
	log.Info("TestCaseController||entry||action=%s", action)

	var rsp HttpRsp

	if _, exists := IsLogin(this); exists == false {
		log.Error("TestCaseController||not login")
		rsp.ErrCode = 2
		rsp.ErrMsg = "not login"
		this.Data["json"] = rsp
		this.ServeJSON()
		return
	}

	var err error
	switch action {
	case "add":
		err = this.addTestCase(&rsp)
	case "delete":
		err = this.deleteTestCase(&rsp)
	case "update":
		err = this.updateTestCase(&rsp)
	case "query":
		err = this.queryTestCase(&rsp)
	case "trigger":
		err = this.trigger(&rsp)
	default:
		rsp.ErrCode = 2
		rsp.ErrMsg = "url error"
		rsp.Data = this.Ctx.Input.Param(":splat")
	}

	if err != nil {
		rsp.ErrCode = 1
		rsp.ErrMsg = "internel error"
	}

	this.Data["json"] = rsp
	this.ServeJSON()
}

func (this *TestCaseController) testCaseReqToDb(req *AddTestCaseReq, tc *dao.TestCase) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	tc.IpId = req.IpId
	bytes, err := json.Marshal(req.Reqs)
	if err != nil {
		log.Error("testCaseReqToDb||json.Marshal failed||req.Reqs=%+v", req.Reqs)
		return err
	}
	tc.ClientData = string(bytes)

	bytes, err = json.Marshal(req.Rsps)
	if err != nil {
		log.Error("testCaseReqToDb||json.Marshal failed||req.Rsps=%+v", req.Rsps)
		return err
	}
	tc.ServerData = string(bytes)
	return nil
}

func (this *TestCaseController) addTestCase(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	var req AddTestCaseReq
	if err := json.Unmarshal(this.Ctx.Input.RequestBody, &req); err != nil {
		log.Error("json.Unmarshal AddTestCaseReq body failed, err=%s", err.Error())
		return err
	} else {
		log.Info("TestCaseController||Add TestCase||req=%+v", req)
	}

	var testCase dao.TestCase
	if err := this.testCaseReqToDb(&req, &testCase); err != nil {
		log.Error("addTestCase||testCaseReqToDb||err=%s", err.Error())
		return err
	}

	//insert db
	id, err := dao.AddTestCase(&testCase)
	if err != nil {
		log.Error("addTestCase||dao.AddTestCase||err=%s", err.Error())
		return err
	}

	var data AddTestCaseRsp
	data.TcId = id

	rsp.ErrCode = 0
	rsp.ErrMsg = "ok"
	rsp.Data = data
	return nil
}

func (this *TestCaseController) testCaseDbToRsp(db *dao.TestCase, testcase *TestCase) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	testcase.TcId = db.Id
	if err := json.Unmarshal([]byte(db.ClientData), &testcase.Reqs); err != nil {
		log.Error("json.Unmarshal testcase.Reqs failed, err=%s", err.Error())
		return err
	}

	if err := json.Unmarshal([]byte(db.ServerData), &testcase.Rsps); err != nil {
		log.Error("json.Unmarshal testcase.Rsps failed, err=%s", err.Error())
		return err
	}

	return nil
}

func (this *TestCaseController) queryTestCase(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	var req QueryTestCaseReq
	if err := json.Unmarshal(this.Ctx.Input.RequestBody, &req); err != nil {
		log.Error("json.Unmarshal QueryTestCaseReq  body failed, err=%s", err.Error())
		return err
	} else {
		log.Info("TestCaseController||query TestCase||req=%+v", req)
	}

	list, err := dao.GetTestCaseByIpId(req.IpId)
	if err != nil {
		log.Error("Query TestCase||GetTestCaseByIpId||Ipid=%d||err=%s", req.IpId, err.Error())
		return err
	} else if list == nil {
		log.Error("Query TestCase||GetTestCaseByIpId||no row||||Ipid=%d", req.IpId)
		rsp.ErrCode = 2
		rsp.ErrMsg = "no interfacePatterm"
		return nil
	}

	var data QueryTestCaseRsp
	for _, value := range *list {
		var testCase TestCase
		if err := this.testCaseDbToRsp(&value, &testCase); err != nil {
			log.Error("queryTestCase||testCaseDbToRsp||err=%s", err.Error())
			return err
		}
		data.TestCases = append(data.TestCases, testCase)
	}

	rsp.ErrCode = 0
	rsp.ErrMsg = "ok"
	rsp.Data = data
	return nil
}

func (this *TestCaseController) trigger(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	var req TriggerReq
	if err := json.Unmarshal(this.Ctx.Input.RequestBody, &req); err != nil {
		log.Error("json.Unmarshal TriggerReq  body failed, err=%s", err.Error())
		return err
	} else {
		log.Info("TestCaseController||trigger||req=%+v", req)
	}

	testCaseDb, err := dao.GetTestCaseByTcId(req.TcId)
	if err != nil {
		log.Error("trigger||Db||err=%s", err.Error())
		return err
	}
	var testCase TestCase
	if err := this.testCaseDbToRsp(testCaseDb, &testCase); err != nil {
		log.Error("trigger||testCaseDbToRsp||err=%s", err.Error())
		return err
	}

	//filldata
	for index, value := range testCase.Reqs {
		inter := value.InterfaceName + "#" + strconv.Itoa(index)
		err := kclient.FillData(0, value.ServiceName, inter, value.Data)
		if err != nil {
			log.Error("trigger||rpc FillData error||struct=%+v||err=%s", value, err.Error())
			return err
		}
	}
	for _, value := range testCase.Rsps {
		err := kclient.FillData(1, value.ServiceName, value.InterfaceName, value.Data)
		if err != nil {
			log.Error("trigger||rpc FillData error||struct=%+v||err=%s", value, err.Error())
			return err
		}
	}
	//trigger
	var data TriggerRsp
	for _, value := range testCase.Reqs {
		serviceNameId, err := this.getServiceNameId(&value)
		if err != nil {
			log.Error("trigger||getServiceNameId||struct=%+v||err=%s", value, err.Error())
			return err
		}
		data.Rsp, err = kclient.RequestTrigger(serviceNameId, value.InterfaceName)
		if err != nil {
			log.Error("trigger||RequestTrigger failed||err=%s", err.Error())
			return err
		}
		break
	}

	rsp.ErrCode = 0
	rsp.ErrMsg = "ok"
	rsp.Data = data
	return nil
}

func (this *TestCaseController) getServiceNameId(structData *StructData) (snId string, err error) {
	service, err := dao.GetServiceByName(structData.ServiceName)
	if err != nil {
		return "", err
	}

	snId = service.Name + "&" + service.Version + "&" + strconv.Itoa(int(service.Port)) + "&" + service.BuffTransport + "&" + service.Protocol
	return snId, nil
}

func (this *TestCaseController) deleteTestCase(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	return nil
}
func (this *TestCaseController) updateTestCase(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	return nil
}
