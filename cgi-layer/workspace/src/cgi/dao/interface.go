package dao

import (
	"github.com/astaxie/beego/logs"
	"github.com/astaxie/beego/orm"
)

func AddInterface(inter *Interface) (id int64, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)
	o := orm.NewOrm()

	if id, err := o.Insert(inter); err != nil {
		log.Error("DB||Interface||insert error||interface=%+v||err=%s", *inter, err.Error())
		return -1, err
	} else {
		log.Info("DB||Interface||insert succ||id=%s", id)
		return id, nil
	}
}

func GetInterfaceByServiceId(sid int64) (inters *[]Interface, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()

	var list []Interface
	num, err := o.Raw("Select * from Interface where serviceId = ?", sid).QueryRows(&list)
	if err != nil {
		log.Error("DB||Interface||query err||serviceId=%d||err=%s", sid, err.Error())
		return nil, err
	}
	if num == 0 {
		log.Info("DB||Interface||query empty||sid=%d", sid)
		return nil, nil
	}
	return &list, nil
}

func GetInterfaceByServiceIdAndMethod(sid int64, method string) (inter *Interface, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()

	var list []Interface
	_, err = o.Raw("Select * from InterfaceMetaData where serviceId = ?", sid).QueryRows(&list)
	if err != nil {
		log.Error("DB||Interface||query err||serviceId=%d||err=%s", sid, err.Error())
		return nil, err
	}

	for _, value := range list {
		if value.InterfaceName == method {
			return &value, nil
		}
	}
	return nil, orm.ErrNoRows
}

func GetInterfaceByInterfaceId(id int64) (inter *Interface, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()
	inter = &Interface{Id: id}
	err = o.Read(inter)

	if err != nil {
		log.Error("DB||Interface||query error||id=%d||err=%s", id, err.Error())
		return nil, err
	}
	return inter, nil
}
