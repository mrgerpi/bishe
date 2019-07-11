#!/bin/sh
# $1: idl file path, $2: serviceName, $3: port

interServiceName=`cat $1 | grep -w '^service' | awk -F " " '{print $2}' | tr -d '{'`
serviceName=$2
port=$3
idlNamespace=`cat $1 | grep namespace | grep cpp | awk -F " " '{print $3}'`
nsArrStr=`echo ${idlNamespace}|awk -F "." '{for(i=1;i<=NF;i++) print $i}'`
nsArray=(${nsArrStr})
for ((i=0;i<${#nsArray[@]};i++))
do
	namespace=${namespace}"::"${nsArray[i]}
done

client_file=`pwd`"/thrift_gen/"${serviceName}"/"${interServiceName}"Client.cpp"
rm -f ${client_file}
touch ${client_file}

#replace INTER_SERVICE_NAME NAME_SPACE with real interface service name, namespace
#include insert client_header.cpp
sed -e "s/INTER_SERVICE_NAME/${interServiceName}/g" -e "s/NAME_SPACE/${namespace}/g" snippet/client_header.cpp > ${client_file}


#get method line num array
skeleton_file=`pwd`"/thrift_gen/"${serviceName}"/"${interServiceName}"_server.skeleton.cpp"
methodLine=`cat ${skeleton_file}|grep -n void|grep _return|grep const|awk -F ":" '{print $1}'`

lineArray=(${methodLine})
#for every method
for ((i=0;i<${#lineArray[@]};i++))
do
	num=${lineArray[i]}
	methodName=`sed -n "${num}p" ${skeleton_file}|awk -F " " '{print $2}'|awk -F "(" '{print $1}'`
	rsp_type=`sed -n "${num}p" ${skeleton_file}|awk -F "&" '{print $1}'|awk -F "(" '{print $2}'`

	if [ $i == 0 ];then
		echo ${methodName}
	else
		echo ${methodName}
	fi

	#insert methdod head
	sed -e "s/INTER_SERVICE_NAME/${interServiceName}/g" -e "s/SERVICE_NAME/${serviceName}/g" \
		-e "s/PORT/${port}/g" -e "s/RESPONSE_TYPE/${rsp_type}/g" \
		-e "s/METHOD/${methodName}/g" snippet/client_method_head.cpp >> ${client_file}

	#get req type list
	all_req_type=`sed -n "${num}p" ${skeleton_file}|awk -F "," '{for(i=2;i<=NF;i++) print $i}' | awk -F " " '{print $2}' | tr -d '&'`
	req_type_array=(${all_req_type})

	#for every req args
	all_req=""
	dataPrefix=`pwd`"/../data/"
	for ((j=0;j<${#req_type_array[@]};j++))
	do
		req_type=${req_type_array[j]}
		req_name="req"$j	#form left to right , req0, req1, req2 ...
		all_req=${all_req}", "${req_name}
		req_path=${dataPrefix}${serviceName}"/"${methodName}"/"${req_name}".json"	
		sed -e "s/SERVICE_NAME/${serviceName}/g" -e "s/METHOD/${methodName}/g" \
			-e "s/REQ_TYPE/${req_type}/g" -e "s#REQ_PATH#${req_path}#g"		\
			-e "s/REQ_NAME/${req_name}/g" snippet/client_args.cpp >> ${client_file}
	done

	#insert method foot
	rsp_path=${dataPrefix}${serviceName}"/"${methodName}"/rsp.json"
	sed -e "s/SERVICE_NAME/${serviceName}/g" -e "s/METHOD/${methodName}/g" \
		-e "s/RSP_TYPE/${rsp_type}/g" -e "s#RSP_PATH#${rsp_path}#g"		\
		-e "s/ALL_REQ/${all_req}/g" snippet/client_tail.cpp >> ${client_file}
done

echo 0
