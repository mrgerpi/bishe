.PHONY: all test clean
root=../../
INC=-I${root}/thirdparty/simple-log/include/ \
	-I ./dependency/curl/include/		\
	-I ./dependency/c-ares/include 	\
	-I ./dependency/json-cpp/include/  \
	-I  ./include 
#json-cpp: json 序列化组件
#curl: 使用了其中的html 转码函数
#c-ares: need by curl, 用户DNS编解码

all:
	mkdir -p include
	mkdir -p lib
	cp src/*.h include/
	g++ -g -c  ${INC} -I src src/http_parser.cpp -o http_parser.o
	g++ -g -c  ${INC} -I src src/http_server.cpp -o http_server.o
	g++ -g -c  ${INC} -I src src/epoll_socket.cpp -o epoll_socket.o
	ar -rcs libsimpleserver.a *.o
	mv libsimpleserver.a lib/
	rm -rf *.o
	
clean:
	rm -rf include
	rm -rf lib

