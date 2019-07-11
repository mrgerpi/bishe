dir_name=`pwd`
export GOROOT=${dir_name}/cgi-layer/root
export GOPATH=${dir_name}/cgi-layer/workspace
export PATH=${GOPATH}/cgi-layer/workspace/bin:${PATH}
echo $PATH
cd ${GOPATH}/src/cgi&&bee run
