#!/bin/bash
#1: idlFile; 2: struct name

idlFile=$1
structName=$2

begin=`cat $idlFile|grep -n ${structName}|grep "struct"|awk -F ":" '{print $1}'`
end=`cat $idlFile |sed -n "${begin},$ p"|grep -m 1 -n }|awk -F ":" '{print $1}'`
end=`expr ${end} + ${begin}`
begin=`expr ${begin} + 1`
end=`expr ${end} - 2`

cat $idlFile|sed -n "${begin},${end} p"|awk -F ";" '{print $1}'|awk -F " " '{print $NF;print $(NF - 1)}'

