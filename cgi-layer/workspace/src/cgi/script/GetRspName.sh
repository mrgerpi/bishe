#!/bin/bash
#1: idlFile; 2: method name

idlFile=$1
methodName=$2

begin=`cat $idlFile|grep -n  service|grep {|awk -F ":" '{print $1}'`
end=`cat $idlFile|sed -n "${begin},$ p"|grep -n }|awk -F ":" '{print $1}'`
end=`expr ${end} + ${begin}`

cat $idlFile|sed -n "${begin},${end} p" | grep ${methodName}|awk -F " " '{print $1}'
