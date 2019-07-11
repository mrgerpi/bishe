dir_name=`pwd`
LIB_PATH=${dir_name}/thirdparty/simple-log/lib:${dir_name}/thirdparty/config/lib:${dir_name}/thirdparty/libevent/lib:${dir_name}/thirdparty/thrift/lib/so:${dir_name}/service_lib

#set LD_LIBARAY_PATH
if [ -z "${LD_LIBRARY_PATH}" ]; then
	export LD_LIBRARY_PATH="${LIB_PATH}"
else
	export LD_LIBRARY_PATH="${LIB_PATH}:${LD_LIBRARY_PATH}"
fi


start() {
	pids=`ps -ef | grep thrift-test | grep -vE 'tmux|grep' | awk '{print $2}'`
	if test -n "$pids";then
		echo $pids | xargs kill -9
	fi
	./thrift-test > /dev/null 2>&1 &
}

stop() {
	pids=`ps -ef | grep thrift-test | grep -vE 'tmux|grep' | awk '{print $2}'`
	if test -n "$pids";then
		echo $pids | xargs kill -9
	fi
}

case "$1" in
	start)
		start
		echo "Done!"
		;;
	stop)
		stop
		echo "Done!"
		;;
	restart)
		stop
		start
		echo "Done!"
		;;
	*)
		echo "Usage: $0 {start|stop}"
		;;
esac
