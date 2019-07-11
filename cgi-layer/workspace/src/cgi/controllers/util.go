package controllers

import (
	"cgi/dao"
	"crypto/md5"
	"fmt"
	"github.com/astaxie/beego/logs"
	"io"
	"io/ioutil"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

type controllerInterface interface {
	GetSession(interface{}) interface{}
}

func IsLogin(this controllerInterface) (uid int64, exist bool) {
	data := this.GetSession("uid")
	if data == nil {
		exist = false
		uid = -1
	} else {
		exist = true
		uid = data.(int64)
	}
	return uid, exist
}

func GetIdlFileName(serviceName string) (string, error) {
	dir, err := filepath.Abs(filepath.Dir(os.Args[0]))
	if err != nil {
		return "", err
	}
	targetFile := strings.Replace(dir, "\\", "/", -1) + "/../../../../src/idl/" + serviceName + ".thrift"
	return targetFile, nil
}

func ServiceAdapterDbToRsp(src *dao.Service, target *Service) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	target.ServiceName = src.Name
	target.Port = src.Port
	target.Transport = src.Transport
	target.BuffTransport = src.BuffTransport
	target.Protocol = src.Protocol
	target.Ip = src.Ip

	//idl
	idlPath := src.Idl
	bytes, err := ioutil.ReadFile(idlPath)
	if err != nil {
		log.Error("ServiceAdapterDbToRsp||read file error||err=%s", err.Error())
		return err
	}

	target.Idl = strings.Replace(string(bytes), "\n", "@", -1)
	return nil
}

func ServiceIdGen(s *dao.Service) string {
	return s.Name + "&" + s.Version + "&" + fmt.Sprintf("%d", s.Port) + "&" +
		s.BuffTransport + "&" + s.Protocol
}

func StrSliceContains(src []string, target string) bool {
	res := false
	for _, value := range src {
		if value == target {
			res = true
			break
		}
	}
	return res
}

func ServiceAdapterReqToDb(src *Service, target *dao.Service) error {
	log := logs.NewLogger()
	log.EnableFuncCallDepth(true)

	target.Ip = src.Ip
	target.Name = src.ServiceName
	//target.Idl = src.Idl
	target.Port = src.Port
	target.Transport = src.Transport
	target.BuffTransport = src.BuffTransport
	target.Protocol = src.Protocol

	h := md5.New()
	io.WriteString(h, src.Idl)
	target.Version = fmt.Sprintf("%x", h.Sum(nil))

	//idl
	var file string
	var err error
	if file, err = GetIdlFileName(target.Name); err != nil {
		log.Error("ServiceAdapter||get path error||err=%s", err.Error())
		return err
	}
	src.Idl = strings.Replace(src.Idl, "@", "\n", -1)
	ioutil.WriteFile(file, []byte(src.Idl), 0644)
	target.Idl = file

	return nil
}

func SplitJoin(tokens []int64, seperator string) string {
	if len(tokens) == 0 {
		return ""
	}

	res := strconv.FormatInt(tokens[0], 10)
	for i := 1; i < len(tokens); i++ {
		res = res + seperator + strconv.FormatInt(tokens[i], 10)
	}
	return res
}

func StrToIntSlice(str []string) (res []int64) {
	for _, value := range str {
		v, _ := strconv.ParseInt(value, 0, 64)
		res = append(res, v)
	}
	return res
}
