package dao

import (
	"github.com/astaxie/beego/logs"
	"github.com/astaxie/beego/orm"
)

func AddService(service *Service) (id int64, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)
	o := orm.NewOrm()

	if id, err := o.Insert(service); err != nil {
		log.Error("DB||Service||insert error||service=%+v||err=%s", *service, err.Error())
		return -1, err
	} else {
		log.Info("DB||Service||insert succ||id=%s", id)
		return id, nil
	}
}

func GetServiceById(id int64) (service *Service, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()
	service = &Service{Id: id}
	err = o.Read(service)

	if err != nil {
		log.Error("DB||Service||query error||id=%d||err=%s", id, err.Error())
		return nil, err
	}
	return service, nil
}

func GetServicesByIds(ids []int64) (services *[]Service, err error) {
	var ss []Service
	for _, id := range ids {
		service, err := GetServiceById(id)
		if err != nil {
			return nil, err
		}
		ss = append(ss, *service)
	}
	return &ss, nil
}

func GetServiceByName(name string) (service *Service, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()

	var s Service
	err = o.Raw("select * from ServiceMetaData where serviceName = ?", name).QueryRow(&s)
	if err != nil {
		log.Error("DB||Service||query err||name=%s||err=%s", name, err.Error())
		return nil, err
	}

	return &s, nil
}
