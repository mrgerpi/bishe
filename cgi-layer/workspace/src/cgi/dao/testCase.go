package dao

import (
	"github.com/astaxie/beego/logs"
	"github.com/astaxie/beego/orm"
)

func AddTestCase(tc *TestCase) (id int64, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)
	o := orm.NewOrm()

	if id, err := o.Insert(tc); err != nil {
		log.Error("DB||TestCase||insert error||testCase=%+v||err=%s", *tc, err.Error())
		return -1, err
	} else {
		log.Info("DB||TestCase||insert succ||id=%s", id)
		return id, nil
	}
}

func GetTestCaseByIpId(ipId int64) (res *[]TestCase, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()

	var list []TestCase
	num, err := o.Raw("Select * from TestCase where interfacePattermId = ?", ipId).QueryRows(&list)
	if err != nil {
		log.Error("DB||TestCase||query err||interfacePattermId=%d||err=%s", ipId, err.Error())
		return nil, err
	}
	if num == 0 {
		log.Info("DB||TestCase||query empty||ipId=%d", ipId)
		return nil, nil
	}
	return &list, nil

}
func GetTestCaseByTcId(tcId int64) (tc *TestCase, err error) {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	o := orm.NewOrm()
	tc = &TestCase{Id: tcId}
	err = o.Read(tc)

	if err != nil {
		log.Error("DB||TestCase||query error||id=%d||err=%s", tcId, err.Error())
		return nil, err
	}
	return tc, nil
}
