dir_name=`pwd`
LIB_PATH=${dir_name}/thirdparty/simple-log/lib:${dir_name}/thirdparty/libevent/lib

#set LD_LIBARAY_PATH
if [ -z "${LD_LIBRARY_PATH}" ]; then
	export LD_LIBRARY_PATH="${LIB_PATH}"
else
	export LD_LIBRARY_PATH="${LIB_PATH}:${LD_LIBRARY_PATH}"
fi

./client
