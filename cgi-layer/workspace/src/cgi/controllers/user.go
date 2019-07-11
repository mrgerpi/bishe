package controllers

import (
	"cgi/dao"
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/logs"

	"encoding/json"
	"strings"
)

type UserController struct {
	beego.Controller
}

func (this *UserController) Post() {
	log := logs.NewLogger()
	log.Debug("UserController||methond=Post||req=%s", string(this.Ctx.Input.RequestBody))
	var req UserLoginReq
	var rsp HttpRsp
	if err := json.Unmarshal(this.Ctx.Input.RequestBody, &req); err != nil {
		log.Error("json.Unmarshal req body failed")
		return
	}

	if user, err := dao.GetUserInfo(req.Email); err != nil {
		rsp.ErrCode = 2
		rsp.ErrMsg = "db error"
		log.Debug("UserController||methond=Post||db error")
	} else {
		if user == nil {
			rsp.ErrCode = 1
			rsp.ErrMsg = "user not exists"
			log.Debug("UserController||methond=Post||user not exists")
		} else {
			if strings.Compare(user.Password, req.Password) == 0 {
				_, exists := IsLogin(this)
				if exists == true {
					rsp.Data = "user login already"
				} else {
					rsp.Data = "login succ"
					this.SetSession("uid", user.Id)
				}

				rsp.ErrCode = 0
				rsp.ErrMsg = "ok"
				log.Debug("UserController||methond=Post||user login succ")

				/*
					v := this.GetSession("asta")
					if v == nil {
						this.SetSession("asta", int(1))
						rsp.Data = this.GetSession("asta")
					} else {
						this.SetSession("asta", v.(int)+1)
						rsp.Data = this.GetSession("asta")
					}
				*/
			} else {
				rsp.ErrCode = 1
				rsp.ErrMsg = "password wrong"
				log.Debug("UserController||methond=Post||password wrong")
			}
		}
	}

	this.Data["json"] = rsp
	this.ServeJSON()
}
