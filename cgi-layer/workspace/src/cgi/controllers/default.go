package controllers

import (
	"cgi/dao"
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/logs"

	"encoding/json"
	"strings"
)

type MainController struct {
	beego.Controller
}

type HelloController struct {
	beego.Controller
}

type UserController struct {
	beego.Controller
}

func (c *HelloController) Get() {
	c.Ctx.WriteString("helloWorld")
	log := logs.NewLogger(10000)
	log.Debug("/hello is call")
}
func (c *UserController) Post() {
	log := logs.NewLogger()
	log.Debug("UserController||methond=Post||req=%s", string(c.Ctx.Input.RequestBody))
	var req UserReq
	if err := json.Unmarshal(c.Ctx.Input.RequestBody, &req); err != nil {
		log.Error("json.Unmarshal req body failed")
		return
	}
	bs, _ := json.Marshal(req)
	log.Debug("email=%s, passwrod=%s, json=%s", req.Email, req.Password, string(bs))

	if user, err := dao.GetUserInfo(req.Email); err != nil {
		c.Ctx.WriteString("user login failed, db error")
		log.Debug("UserController||methond=Post||user login failed")
		return
	} else {
		if user == nil {
			c.Ctx.WriteString("user login failed, user not exists")
			log.Debug("UserController||methond=Post||user not exists")
		} else {
			if strings.Compare(user.Password, req.Password) == 0 {
				c.Ctx.WriteString("user login succ")
				log.Debug("UserController||methond=Post||user login succ")
			} else {
				c.Ctx.WriteString("user login failed, password wrong")
				log.Debug("UserController||methond=Post||password wrong")
			}
		}
	}
}

func (c *MainController) Get() {
	c.Data["Website"] = "beego.me"
	c.Data["Email"] = "astaxie@gmail.com"
	c.TplName = "index.tpl"
}
