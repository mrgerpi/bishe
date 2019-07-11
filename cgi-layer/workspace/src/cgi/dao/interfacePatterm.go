package dao

import (
	"github.com/astaxie/beego/logs"
	"github.com/astaxie/beego/orm"
)

func AddInterfacePatterm(ip *InterfacePatterm) (id int64, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()

	if id, err := o.Insert(ip); err != nil {
		log.Error("DB||InterfacePatterm||insert error||err=%s", err.Error())
		return -1, err
	} else {
		log.Info("DB||InterfacePatterm||insert succ||id=%s", id)
		return id, nil
	}
}

func GetInterfacePattermBySpId(spId int64) (*[]InterfacePatterm, error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()

	var list []InterfacePatterm
	num, err := o.Raw("select * from InterfacePatterm where servicePattermId = ?", spId).QueryRows(&list)
	if err != nil {
		log.Error("DB||InterfacePatterm||query err||spid=%d||err=%s", spId, err.Error())
		return nil, err
	}
	if num == 0 {
		log.Info("DB||InterfacePatterm||query empty||spid=%d", spId)
		return nil, nil
	}
	return &list, nil

}
