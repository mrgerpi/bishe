package main

import (
	_ "cgi/routers"
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/context"
)

func init() {
	var FilterNoCache = func(ctx *context.Context) {
		ctx.Output.Header("Cache-Control", "no-cache, no-store, must-revalidate")
		ctx.Output.Header("Pragma", "no-cache")
		ctx.Output.Header("Expires", "0")
	}

	beego.InsertFilter("/static/*", beego.BeforeStatic, FilterNoCache)
}

func main() {
	beego.BConfig.CopyRequestBody = true
<<<<<<< HEAD
	beego.BConfig.WebConfig.Session.SessionOn = true
	beego.BConfig.WebConfig.AutoRender = false
	beego.BConfig.WebConfig.Session.SessionGCMaxLifetime = 60 * 60 * 24
=======
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	beego.Run()
}
