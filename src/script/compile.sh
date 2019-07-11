#!/bin/sh
# $1: idl file path, $2: serviceName 
# $3: type; 0: server, 1: client, 2: server that client has been compiled, 3: client that server has been compiled

interServiceName=`cat $1 | grep -w ^service | awk -F " " '{print $2}' | tr -d '{'`
serviceName=$2
file=`pwd`"/script/makefile"

case $3 in
	0)
		make -f ${file} serviceName=${serviceName} interServiceName=${interServiceName} Server
		;;
	1)
		make -f ${file} serviceName=${serviceName} interServiceName=${interServiceName} Client 
		;;
	2)
		make -f ${file} serviceName=${serviceName} interServiceName=${interServiceName} Only_Server 
		;;
	3)
		make -f ${file} serviceName=${serviceName} interServiceName=${interServiceName} Only_Client 
		;;
	*)
		echo 1
		;;
esac
