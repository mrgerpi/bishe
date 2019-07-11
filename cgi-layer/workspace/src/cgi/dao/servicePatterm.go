package dao

import (
	"github.com/astaxie/beego/logs"
	"github.com/astaxie/beego/orm"
)

func AddServicePatterm(sp *ServicePatterm) (id int64, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()

	if id, err := o.Insert(sp); err != nil {
		log.Error("DB||ServicePatterm||insert error||err=%s", err.Error())
		return -1, err
	} else {
		log.Info("DB||ServicePatterm||insert succ||id=%s", id)
		return id, nil
	}
}

func GetServicePattermsByUid(uid int64) (list []ServicePatterm, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()

	num, err := o.Raw("Select * from ServicePatterm where userId = ?", uid).QueryRows(&list)
	if err != nil {
		log.Error("DB||ServicePatterm||query err||uid=%d||err=%s", uid, err.Error())
		return nil, err
	}
	if num == 0 {
		log.Info("DB||ServicePatterm||query empty||uid=%d", uid)
		return nil, nil
	}
	return list, nil
}

func GetServicePattermBySpId(spid int64, sp *ServicePatterm) (err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()

	err = o.Raw("Select * from ServicePatterm where servicePattermId = ?", spid).QueryRow(sp)
	if err != nil {
		log.Error("DB||ServicePatterm||query err||spid=%d||err=%s", spid, err.Error())
		return err
	}
	return nil
}
