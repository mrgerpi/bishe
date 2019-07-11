package routers

import (
	"cgi/controllers"
	"github.com/astaxie/beego"
)

func init() {
<<<<<<< HEAD
	beego.Router("/user/login", &controllers.UserController{})
	beego.Router("/project/*", &controllers.ProjectController{}, "get,post:ProjectEntry")
	beego.Router("/interface/*", &controllers.InterfaceController{}, "get,post:InterfaceEntry")
	beego.Router("/testcase/*", &controllers.TestCaseController{}, "get,post:TestCaseEntry")
=======
	beego.Router("/", &controllers.MainController{})
	beego.Router("/hello", &controllers.HelloController{})
	beego.Router("/user", &controllers.UserController{})
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
}
