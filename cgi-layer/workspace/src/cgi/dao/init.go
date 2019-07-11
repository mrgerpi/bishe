package dao

import (
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/logs"
	"github.com/astaxie/beego/orm"
	_ "github.com/go-sql-driver/mysql"
)

func init() {
	// 需要在init中注册定义的model
	orm.RegisterModel(new(User))
	orm.RegisterModel(new(Service))
	orm.RegisterModel(new(ServicePatterm))
	orm.RegisterModel(new(Interface))
	orm.RegisterModel(new(InterfacePatterm))
	orm.RegisterModel(new(TestCase))

	orm.RegisterDriver("mysql", orm.DRMySQL)

	dbUser := beego.AppConfig.String("mysql::user")
	dbPassword := beego.AppConfig.String("mysql::password")
	dbName := beego.AppConfig.String("mysql::databaseName")
	addr := beego.AppConfig.String("mysql::addr") + ":" + beego.AppConfig.String("mysql::port")
	url := dbUser + ":" + dbPassword + "@tcp(" + addr + ")/" + dbName + "?charset=utf8"
	log := logs.NewLogger()
	log.Debug("database_init||url=" + url)
	//orm.RegisterDataBase("default", "mysql", "root:root@/orm_test?charset=utf8")
	orm.RegisterDataBase("default", "mysql", url)

	maxIdleConns, err := beego.AppConfig.Int("mysql::maxIdleConns")
	if err != nil {
		log.Debug("database_init||parse mysql::maxIdleConns failed||error=%s", err.Error())
		return
	}
	maxOpenConns, err := beego.AppConfig.Int("mysql::maxOpenConns")
	if err != nil {
		log.Debug("database_init||parse mysql::maxOpenConns failed||error=%s", err.Error())
		return
	}

	orm.SetMaxIdleConns("default", maxIdleConns)
	orm.SetMaxOpenConns("default", maxOpenConns)

	orm.Debug = true
}
