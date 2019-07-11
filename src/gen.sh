function fail() {
<<<<<<< HEAD
	echo "Error: "thrift_test_kernel	
	exit 1
}


filename="./idl/thrift_test_kernel.thrift"
=======
	echo "Error: "$1	
	exit 1
}

if test -z $1;then
	fail "no service name"
fi

filename="./idl/"$1".thrift"
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
if ! test -r $filename; then
	fail "file "$filename" not exists"	
fi

echo "thrift compile $filename ..."
<<<<<<< HEAD
rm -rf thrift_gen/thrift_test_kernel
mkdir -p thrift_gen/thrift_test_kernel
/home/Shit/thrift-test/src/thirdparty/thrift/bin/thrift -r -gen cpp -out thrift_gen/thrift_test_kernel	$filename
rm -rf ../kernel_client/thrift_gen/thrift_test_kernel
cp -r thrift_gen/thrift_test_kernel ../kernel_client/thrift_gen/
=======
rm -rf thrift_gen/$1
mkdir -p thrift_gen/$1
/home/Shit/thrift-test/src/thirdparty/thrift/bin/thrift -r -gen cpp -out thrift_gen/$1	$filename
rm -rf ../kernel_client/thrift_gen/$1
cp -r thrift_gen/$1 ../kernel_client/thrift_gen/
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
