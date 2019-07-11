package controllers

import (
	"cgi/dao"
	kclient "cgi/kernel_client"
	"encoding/json"
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/logs"
	"strconv"
	"strings"
	/*
	 */)

type ProjectController struct {
	beego.Controller
}

func (this *ProjectController) ProjectEntry() {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	action := this.Ctx.Input.Param(":splat")
	log.Info("ProjectController||entry||action=%s", action)

	var rsp HttpRsp

	if _, exists := IsLogin(this); exists == false {
		log.Error("ProjectController||not login")
		rsp.ErrCode = 2
		rsp.ErrMsg = "not login"
		this.Data["json"] = rsp
		this.ServeJSON()
		return
	}

	var err error
	switch action {
	case "add":
		err = this.addProject(&rsp)
	case "delete":
		err = this.delProject(&rsp)
	case "update":
		err = this.updateProject(&rsp)
	case "query":
		err = this.queryProject(&rsp)
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

func (this *ProjectController) addProject(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	var req AddProjectReq
	if err := json.Unmarshal(this.Ctx.Input.RequestBody, &req); err != nil {
		log.Error("json.Unmarshal AddProjectReq body failed, err=%s", err.Error())
		return err
	} else {
		log.Info("ProjectController||Add Project||req=%+v", req)
	}

	//add service to db and kernel
	var service dao.Service
	ServiceAdapterReqToDb(&(req.Client), &service)

	if err := kclient.AddService(0, &service); err != nil {
		log.Error("ProjectController||add service to kernel error||client=%+v||err=%s", service, err.Error())
		return err
	}

	cid, err := dao.AddService(&service)
	if err != nil {
		log.Error("ProjectController||add service to db error||client=%+v||err=%s", service, err.Error())
		return err
	}
	service = dao.Service{}

	sids := make([]int64, len(req.Servers))
	for i := 0; i < len(req.Servers); i++ {
		ServiceAdapterReqToDb(&(req.Servers[i]), &service)

		if err := kclient.AddService(1, &service); err != nil {
			log.Error("ProjectController||add service to kernel error||server=%+v||err=%s", service, err.Error())
			return err
		}

		sid, err := dao.AddService(&service)
		if err != nil {
			log.Error("ProjectController||add service to db error||server=%+v||err=%s", service, err.Error())
			return err
		}
		sids[i] = sid
		service = dao.Service{}
	}

	//add servicePatterm to db
	var sp dao.ServicePatterm
	sp.UserId, _ = IsLogin(this)
	sp.ClientServiceId = cid
	sp.ServerServiceIds = SplitJoin(sids, "_")
	spid, err := dao.AddServicePatterm(&sp)
	if err != nil {
		log.Error("ProjectController||add service patterm  error||serverPatterm=%+v||err=%s", sp, err.Error())
		return err
	}

	var body AddProjectRsp
	body.SpId = spid

	rsp.ErrCode = 0
	rsp.ErrMsg = "ok"
	rsp.Data = body
	return nil
}

func (this *ProjectController) queryProject(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	log.Info("ProjectController||Query Project")

	clientList, err := kclient.GetServiceList(0) //kernel clients
	if err != nil {
		log.Error("ProjectController||query Project||rpc||client GetServcieList||err=%s", err.Error())
		return err
	}
	if clientList == nil || len(clientList) == 0 {
		log.Info("ProjectController||query Project||rpc||client GetServcieList return empty")
		clientList = make([]string, 0)
	}

	serverList, err := kclient.GetServiceList(1) //kernel servers
	if err != nil {
		log.Error("ProjectController||query Project||rpc||server GetServcieList||err=%s", err.Error())
		return err
	}
	if serverList == nil || len(serverList) == 0 {
		log.Info("ProjectController||query Project||rpc||server GetServcieList return empty")
		serverList = make([]string, 0)
	}

	res := make([]Project, 0)

	//get service patterms by userid
	uid, _ := IsLogin(this)
	spList, err := dao.GetServicePattermsByUid(uid)
	if err != nil {
		log.Error("ProjectController||query Project||db||GetServicePattermsByUid")
		return err
	}
	if spList == nil || len(spList) == 0 {
		log.Info("ProjectController||query Project||db||GetServicePattermsByUid Empty")
		rsp.ErrCode = 0
		rsp.ErrMsg = "ok"
		rsp.Data = res
		return nil
	}

	var serviceId string
	for _, value := range spList {
		var p Project
		p.SpId = value.Id

		//client
		client, err := dao.GetServiceById(value.ClientServiceId)
		if err != nil {
			log.Error("ProjectController||query Project||db||GetServiceById")
			return err
		}

		serviceId = ServiceIdGen(client)
		if StrSliceContains(clientList, serviceId) == false {
			if err := kclient.AddService(0, client); err != nil {
				log.Error("ProjectController||add service error||client=%+v||err=%s", *client, err.Error())
				return err
			}
		}

		var rspClient Service
		ServiceAdapterDbToRsp(client, &rspClient)
		p.Client = rspClient

		//servers
		serverIdStrs := strings.Split(value.ServerServiceIds, "_")
		for _, serverIdStr := range serverIdStrs {
			serverId, _ := strconv.ParseInt(serverIdStr, 0, 64)
			server, err := dao.GetServiceById(serverId)
			if err != nil {
				log.Error("ProjectController||query Project||db||GetServiceById")
				return err
			}

			serviceId = ServiceIdGen(server)
			if StrSliceContains(serverList, serviceId) == false {
				if err := kclient.AddService(1, server); err != nil {
					log.Error("ProjectController||add service error||server=%+v||err=%s", &server, err.Error())
					return err
				}
			}

			var rspServer Service
			ServiceAdapterDbToRsp(server, &rspServer)
			p.Servers = append(p.Servers, rspServer)
		}
		res = append(res, p)
	}

	rsp.ErrCode = 0
	rsp.ErrMsg = "ok"
	rsp.Data = res

	return nil
}

func (this *ProjectController) delProject(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)
	log.Info("del project")
	return nil
}

func (this *ProjectController) updateProject(rsp *HttpRsp) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)
	log.Info("update project")
	return nil
}
