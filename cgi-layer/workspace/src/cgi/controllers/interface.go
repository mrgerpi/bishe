package controllers

import (
	"cgi/dao"
	"encoding/json"
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/logs"
	"github.com/astaxie/beego/orm"
	"os/exec"
	"strings"
)

type InterfaceController struct {
	beego.Controller
}

func (this *InterfaceController) InterfaceEntry() {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	action := this.Ctx.Input.Param(":splat")
	log.Info("ProjectController||entry||action=%s", action)

	var rsp HttpRsp

	if _, exists := IsLogin(this); exists == false {
		log.Error("InterfaceController||not login")
		rsp.ErrCode = 2
		rsp.ErrMsg = "not login"
		this.Data["json"] = rsp
		this.ServeJSON()
		return
	}

	var err error
	switch action {
	case "add":
		err = this.addInterface(&rsp)
	case "delete":
		err = this.deleteInterface(&rsp)
	case "update":
		err = this.updateInterface(&rsp)
	case "query":
		err = this.queryInterface(&rsp)
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

func parseFileds(idl string, reqName string) (res *[]Filed, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	cmd := exec.Command("./script/StructParse.sh", idl, reqName)
	output, err := cmd.CombinedOutput()
	if err != nil {
		log.Error("getStructDesc||StructParse.sh exe failed||idl=%s||req_name=%s||output=%s", idl, reqName, string(output))
		return nil, err
	}
	log.Info("getStructDesc||StructDesc.sh exe succ||output=%s", string(output))
	outputs := strings.Split(string(output), "\n")
	var fileds []Filed
	for i := 0; i < len(outputs)-1; i++ {
		var filed Filed
		filed.FiledName = outputs[i]
		i++
		filed.FiledType = outputs[i]
		fileds = append(fileds, filed)
	}
	return &fileds, nil
}

func (this *InterfaceController) getStructDesc(serviceId int64, method string, typeT int) (desc interface{}, interfaceId int64, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	inter, err := dao.GetInterfaceByServiceIdAndMethod(serviceId, method)
	if err == orm.ErrNoRows { //idl parse
		log.Info("getStructDesc||GetInterfaceByServiceIdAndMethod||no rows||serviceId=%d||method=%s", serviceId, method)
		service, err := dao.GetServiceById(serviceId)
		if err != nil {
			log.Error("getStructDesc||getServiceById||Bb error||err=%s", err.Error())
			return nil, -1, err
		}

		var rspStructDesc StructDesc
		var reqStructDescs []StructDesc

		//server.Idl + method -> reqStructDescs
		cmd := exec.Command("./script/GetReqName.sh", service.Idl, method)
		output, err := cmd.CombinedOutput()
		if err != nil {
			log.Error("getStructDesc||GetReqName.sh exe failed||idl=%s||method=%s||output=%s",
				service.Idl, method, string(output))
			return nil, -1, err
		}
		log.Info("getStructDesc||GetReqName.sh exe succ||req_name=%s", string(output))
		reqNames := strings.Split(string(output), "\n")

		var sDesc StructDesc
		sDesc.ServiceName = service.Name
		sDesc.InterfaceName = method

		for _, reqName := range reqNames[0:(len(reqNames) - 1)] {
			sDesc.StructName = reqName
			//parse Fileds
			fileds, err := parseFileds(service.Idl, reqName)
			if err != nil {
				log.Error("getStructDesc||req parse filed fialed||idl=%s||req_name=%s",
					service.Idl, reqName)
				return nil, -1, err
			}
			sDesc.Fileds = *fileds
			reqStructDescs = append(reqStructDescs, sDesc)
		}

		//server.Idl + method -> rspStructDesc
		cmd = exec.Command("./script/GetRspName.sh", service.Idl, method)
		output, err = cmd.CombinedOutput()
		if err != nil {
			log.Error("getStructDesc||GetRspName.sh exe failed||idl=%s||method=%s||output=%s",
				service.Idl, method, string(output))
			return nil, -1, err
		}
		log.Info("getStructDesc||GetRspName.sh exe succ||rsp_name=%s", string(output))
		sDesc.StructName = strings.Split(string(output), "\n")[0]

		fileds, err := parseFileds(service.Idl, sDesc.StructName)
		if err != nil {
			log.Error("getStructDesc||rsp parse filed fialed||idl=%s||rsp_name=%s",
				service.Idl, string(output))
			return nil, -1, err
		}
		sDesc.Fileds = *fileds
		rspStructDesc = sDesc

		//insert into Interface table, get interfaceId
		var interf dao.Interface
		interf.ServiceId = serviceId
		interf.InterfaceName = method
		bytes, _ := json.Marshal(reqStructDescs)
		interf.ReqDesc = string(bytes)
		bytes, _ = json.Marshal(rspStructDesc)
		interf.RspDesc = string(bytes)
		interId, err := dao.AddInterface(&interf)
		if err != nil {
			log.Error("getStructDesc||db||add interface error||interface=%+v", interf)
			return nil, -1, err
		}

		if typeT == 0 {
			return reqStructDescs, interId, nil
		} else if typeT == 1 {
			return rspStructDesc, interId, nil
		}
	} else if err != nil {
		log.Error("getStructDesc||GetInterfaceByServiceId||Bb error||err=%s", err.Error())
		return nil, -1, err
	} else { //read form db
		if typeT == 0 {
			var reqDescs []StructDesc
			json.Unmarshal([]byte(inter.ReqDesc), &reqDescs)
			desc = reqDescs
		} else if typeT == 1 {
			var rspDesc StructDesc
			json.Unmarshal([]byte(inter.RspDesc), &rspDesc)
			desc = rspDesc
		}
		log.Info("getStructDesc||reqDescs json.Unmarshal||result=%+v", desc)
	}
	return desc, inter.Id, nil
}

func (this *InterfaceController) addInterface(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	var req AddInterfaceReq
	if err := json.Unmarshal(this.Ctx.Input.RequestBody, &req); err != nil {
		log.Error("json.Unmarshal AddInterfaceReq body failed, err=%s", err.Error())
		return err
	} else {
		log.Info("InterfaceController||Add Interface||req=%+v", req)
	}

	//ServicePatterm SpId -> ServiceId map
	var sp dao.ServicePatterm
	if err := dao.GetServicePattermBySpId(req.SpId, &sp); err != nil {
		if err == orm.ErrNoRows || err == orm.ErrMissPK {
			log.Error("Add Interface||GetServicePattermBySpId||no row||spid=%d", req.SpId)
			rsp.ErrCode = 2
			rsp.ErrMsg = "servicePattermId wrong"
			return nil
		} else {
			log.Error("Add Interface||GetServicePattermBySpId||err=%s", err.Error())
			return err
		}
	}

	var interPatterm dao.InterfacePatterm //insert db
	interPatterm.SpId = req.SpId

	var rspData AddInterfaceRsp

	//client interface
	sDesc, clientInterfaceId, err := this.getStructDesc(sp.ClientServiceId, req.Interface, 0)
	if err != nil {
		log.Error("Add Interface||get struct desc err||err=%s", err.Error())
		return err
	}
	interPatterm.ClientInterId = clientInterfaceId
	rspData.Reqs, _ = sDesc.([]StructDesc)

	//server Interfaces
	serverServiceIdStrs := strings.Split(sp.ServerServiceIds, "_")
	serverServiceIds := StrToIntSlice(serverServiceIdStrs)
	serverServices, err := dao.GetServicesByIds(serverServiceIds)
	if err != nil {
		log.Error("Add Interface||Db||GetServicesByServiceIds||err=%s", err.Error())
		return err
	}

	var serverInterIds []int64
	for _, depend := range req.Dependency {
		match := false
		for _, serverService := range *serverServices {
			if depend.ServiceName != serverService.Name {
				continue
			}

			sDesc, sInterId, err := this.getStructDesc(serverService.Id, depend.MethodName, 1)
			if err != nil {
				log.Error("Add Interface||get struct desc err||err=%s", err.Error())
				return err
			}
			serverInterIds = append(serverInterIds, sInterId)
			rspDesc, _ := sDesc.(StructDesc)
			rspData.Rsps = append(rspData.Rsps, rspDesc)
			match = true
			break
		}
		if match == false {
			log.Error("Add Interface||Dependency do not match Service||dependency=%+v", depend)
			rsp.ErrCode = 2
			rsp.ErrMsg = "dependency wrong"
			return nil
		}
	}

	serverInterIdsStr := SplitJoin(serverInterIds, "_")
	interPatterm.ServerInterIds = serverInterIdsStr

	//insert interPatterm to db, get Ipid
	ipId, err := dao.AddInterfacePatterm(&interPatterm)
	if err != nil {
		log.Error("Add Interface||Db||AddInterfacePatterm||interPatterm=%+v", interPatterm)
		return err
	}
	rspData.IpId = ipId
	rsp.ErrCode = 0
	rsp.ErrMsg = "ok"
	rsp.Data = rspData
	return nil
}

func (this *InterfaceController) queryInterface(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	var req QueryInterfaceReq
	if err := json.Unmarshal(this.Ctx.Input.RequestBody, &req); err != nil {
		log.Error("json.Unmarshal QueryInterfaceReq  body failed, err=%s", err.Error())
		return err
	} else {
		log.Info("InterfaceController||query Interface||req=%+v", req)
	}

	list, err := dao.GetInterfacePattermBySpId(req.SpId)
	if err != nil {
		log.Error("Query Interface||GetInterfacePattermBySpId||spid=%d||err=%s", req.SpId, err.Error())
		return err
	} else if list == nil {
		log.Error("Query Interface||GetInterfacePattermBySpId||spid=%d||err=%s", req.SpId, err.Error())
		log.Error("Query Interface||GetInterfacePattermBySpId||no row||spid=%d", req.SpId)
		rsp.ErrCode = 2
		rsp.ErrMsg = "no interfacePatterm"
		return nil
	}

	var queryInterfaceRsp []AddInterfaceRsp
	for _, value := range *list {
		var d AddInterfaceRsp
		d.IpId = value.Id

		//unmarshal d.Reqs
		clientInterId := value.ClientInterId
		inter, err := dao.GetInterfaceByInterfaceId(clientInterId)
		if err != nil {
			log.Error("Query Interface||GetInterfaceByInterfaceId||id=%d||err=%s", clientInterId, err.Error())
			return err
		}
		if err := json.Unmarshal([]byte(inter.ReqDesc), &d.Reqs); err != nil {
			log.Error("json.Unmarshal db interface ReqDesc failed, err=%s", err.Error())
			return err
		}

		//unmarshal d.Rsps
		serverInterIdStrs := strings.Split(value.ServerInterIds, "_")
		serverInterIds := StrToIntSlice(serverInterIdStrs)
		for _, serverInterId := range serverInterIds {
			var rspDesc StructDesc

			inter, err := dao.GetInterfaceByInterfaceId(serverInterId)
			if err != nil {
				log.Error("Query Interface||GetInterfaceByInterfaceId||id=%d||err=%s", serverInterId, err.Error())
				return err
			}
			if err := json.Unmarshal([]byte(inter.RspDesc), &rspDesc); err != nil {
				log.Error("json.Unmarshal db interface RspDesc failed, err=%s", err.Error())
				return err
			}
			d.Rsps = append(d.Rsps, rspDesc)
		}

		queryInterfaceRsp = append(queryInterfaceRsp, d)
	}
	rsp.ErrCode = 0
	rsp.ErrMsg = "ok"
	rsp.Data = queryInterfaceRsp
	return nil
}

func (this *InterfaceController) updateInterface(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)
	return nil
}

func (this *InterfaceController) deleteInterface(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)
	return nil
}
