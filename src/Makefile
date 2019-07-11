INCLUDE = -I ./	\
		  -I ./util	\
		  -I ./main 	\
		  -I ./thirdparty/simple-log/include/	\
		  -I ./thirdparty/thrift/include/	\
		  -I ./thirdparty/config/include/	\
		  -I ./thirdparty/libevent/include/	\
		  -I ./thrift_gen/thrift_test_kernel \
		  -I ./thrift_server/thrift_test_kernel 

LIB = ./thirdparty/simple-log/lib/libsimplelog.a	\
	  -L./thirdparty/config/lib -lconfig++		\
	  -L./thirdparty/thrift/lib/so -lthriftnb-0.9.2 -lthrift-0.9.2         \
	  -L./thirdparty/libevent/lib -levent		\
	  -lpthread -ldl -rdynamic

FLAGS = -O0  -g -lrt -std=c++11

OBJ_DIR = ./obj_dir

VPATH = ./	\
		./main	\
		./util	\
		./thrift_gen/thrift_test_kernel \
		./thrift_server/thrift_test_kernel \
		${OBJ_DIR}

cpu_num = $(shell echo $$(nproc))

main_obj = main.o ServiceManager.o ServiceSrcGenerator.o  ServiceLoader.o
util_obj = didiutils.o

thrift_test_kernel_gen_obj = thrift_test_kernel_constants.o ThriftTestKernelService.o thrift_test_kernel_types.o #ThriftTestKernelService_server.skeleton.o 
thrift_test_kernel_server_obj = ThriftTestKernelServiceServer.o ThriftTestKernelServiceHandler.o

all: thrift-test

thrift-test: ${main_obj} ${util_obj} ${thrift_test_kernel_gen_obj} ${thrift_test_kernel_server_obj}
	g++ ${FLAGS} $(patsubst %.o,$(OBJ_DIR)/%.o,$(^F)) $(LIB) -o ./$@ 

%.o:%.cpp 
	@mkdir -p ${OBJ_DIR}
	@echo "Compiling $< ..."
	@g++ -c ${INCLUDE} ${FLAGS} $< -o ${OBJ_DIR}/$@

clean:
	rm -rf ${OBJ_DIR}
	rm thrift-test
