dir_name=$(cd $(dirname "$0")"/../../" && pwd)
export LD_LIBRARY_PATH=${dir_name}/simple-server/dependency/c-ares/lib:${LD_LIBRARY_PATH}
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
./server_test 80 
