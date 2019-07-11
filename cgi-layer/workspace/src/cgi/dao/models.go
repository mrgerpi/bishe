package dao

<<<<<<< HEAD
type User struct {
	Id       int64  `orm:"column(userId);unique"`
=======
import (
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/logs"
	"github.com/astaxie/beego/orm"
	_ "github.com/go-sql-driver/mysql"
)

type User struct {
	Id       int    `orm:"column(userId);unique"`
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	Name     string `orm:"column(userName);unique"`
	Password string `orm:"column(password)"`
}

func (user *User) TableName() string {
	return "User"
}

<<<<<<< HEAD
type Service struct {
	Id            int64  `orm:"column(serviceId);unique"`
	Name          string `orm:"column(serviceName)"`
	Idl           string `orm:"column(idl)"`
	Port          int64  `orm:"column(port)"`
	Transport     string `orm:"column(transport)"`
	BuffTransport string `orm:"column(buffTransport)"`
	Protocol      string `orm:"column(protocol)"`
	Version       string `orm:"column(version)"`
	Ip            string `orm:"column(ip)"`
}

func (service *Service) TableName() string {
	return "ServiceMetaData"
}

type ServicePatterm struct {
	Id               int64  `orm:"column(servicePattermId)"`
	UserId           int64  `orm:"column(userId)"`
	ClientServiceId  int64  `orm:"column(clientServiceId)"`
	ServerServiceIds string `orm:"column(serverServiceIds)"`
}

func (service *ServicePatterm) TableName() string {
	return "ServicePatterm"
}

type Interface struct {
	Id            int64  `orm:"column(interfaceId);unique"`
	ServiceId     int64  `orm:"column(serviceId)"`
	InterfaceName string `orm:"column(interfaceName)"`
	ReqDesc       string `orm:"column(reqDesc)"`
	RspDesc       string `orm:"column(rspDesc)"`
}

func (service *Interface) TableName() string {
	return "InterfaceMetaData"
}

type InterfacePatterm struct {
	Id             int64  `orm:"column(interfacePattermId);unique"`
	SpId           int64  `orm:"column(servicePattermId)"`
	ClientInterId  int64  `orm:"column(clientInterfaceId)"`
	ServerInterIds string `orm:"column(serverInterfaceIds)"`
}

func (service *InterfacePatterm) TableName() string {
	return "InterfacePatterm"
}

type TestCase struct {
	Id         int64  `orm:"column(testCaseId);unique"`
	IpId       int64  `orm:"column(interfacePattermId)"`
	ClientData string `orm:"column(clientInterfaceData)"`
	ServerData string `orm:"column(serverInterfacesData)"`
}

func (service *TestCase) TableName() string {
	return "TestCase"
=======
func init() {
	// 需要在init中注册定义的model
	orm.RegisterModel(new(User))
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
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
}
