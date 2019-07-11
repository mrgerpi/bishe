#!/bin/sh
# $1: idl file path, $2: service name
idl_path=$1
target_path=`pwd`"/thrift_gen/"$2
cur_dir=`pwd`
thrift=${cur_dir}"/thirdparty/thrift/bin/thrift"

rm -rf ${target_path}
mkdir -p ${target_path}

${thrift}  -r -gen cpp -out ${target_path} ${idl_path}
echo $?
