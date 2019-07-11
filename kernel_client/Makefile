INCLUDE = -I ./	\
		  -I ./main 	\
		  -I ./util\
		  -I ./thirdparty/simple-log/include/	\
		  -I ./thirdparty/thrift/include/	\
		  -I ./thrift_gen/thrift_test_kernel \

LIB = ./thirdparty/simple-log/lib/libsimplelog.a	\
	  -L./thirdparty/thrift/lib -lthriftnb -lthrift         \
	  -lpthread 

FLAGS = -O0  -g -lrt -std=c++11

OBJ_DIR = ./obj_dir

VPATH = ./	\
		./main	\
		./util \
		./thrift_gen/thrift_test_kernel \
		./thrift_server/thrift_test_kernel \
		${OBJ_DIR}

main_obj = main.o cmd_parser.o thrift_test_client.o
util_obj = didiutils.o

thrift_test_kernel_gen_obj = thrift_test_kernel_constants.o ThriftTestKernelService.o thrift_test_kernel_types.o #ThriftTestKernelService_server.skeleton.o 

all: client

client: ${main_obj} ${util_obj} ${thrift_test_kernel_gen_obj} 
	g++ ${FLAGS} $(patsubst %.o,$(OBJ_DIR)/%.o,$(^F)) $(LIB) -o ./$@ 

%.o:%.cpp 
	@mkdir -p ${OBJ_DIR}
	@echo "Compiling $< ..."
	@g++ -c ${INCLUDE} ${FLAGS} $< -o ${OBJ_DIR}/$@

clean:
	rm -rf ${OBJ_DIR}
	rm client 
