.PHONY: all test clean
root=../../..
INC= -I${root}/thirdparty/simple-log/include/ \
	-I${root}/thirdparty/simple-server/dependency/json-cpp/include/  \
	-I${root}/thirdparty/simple-server/dependency/curl/include/  \
	-I${root}/thirdparty/simple-server/dependency/c-ares/include/  \
	-I${root}/thirdparty/simple-server/include 

LIB= ../lib/libsimpleserver.a ../dependency/json-cpp/lib/libjson_libmt.a \
	 ${root}/thirdparty/simple-log/lib/libsimplelog.a \
	 -L../dependency/curl/lib -lcurl \
	 -L../dependency/c-ares/lib -lcares \
	 -lpthread

test: http_server_test.cpp http_parser_test.cpp
	g++ -g  ${INC} http_server_test.cpp   ${LIB} -o server_test 
	g++ -g  ${INC} http_parser_test.cpp  ${LIB}  -o parser_test
	
clean:
	rm -rf server_test parser_test 

