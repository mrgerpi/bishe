package dao

import (
	"github.com/astaxie/beego/logs"
	"github.com/astaxie/beego/orm"
	"strings"
<<<<<<< HEAD
=======
	"unsafe"
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
)

func GetUserInfo(userName string) (*User, error) {
	log := logs.NewLogger()
	o := orm.NewOrm()
	var user User

<<<<<<< HEAD
=======
	log.Debug("Sizeof(User{})= %d", unsafe.Sizeof(User{}))

>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	r := o.Raw("Select userId, userName, password from User where userName = ?", userName)
	err := r.QueryRow(&user)
	if err != nil {
		if strings.Compare(err.Error(), "<QuerySeter> no row found") == 0 {
			log.Error("GetUserInfo||user login failed, no user found")
			return nil, nil
		} else {
			log.Error("GetUserInfo||user login failed, db error")
			return nil, err
		}
	} else {
		return &user, nil
	}
}
