INCLUDE = -I ./	\
		  -I ./main 	\
		  -I ./util\
		  -I ./thirdparty/simple-log/include/	\
		  -I ./thirdparty/thrift/include/	\
		  -I ./thrift_gen/calculate\
		  -I ./thrift_gen/test_service

LIB = ./thirdparty/simple-log/lib/libsimplelog.a	\
	  -L./thirdparty/thrift/lib -lthriftnb -lthrift         \
	  -lpthread 

FLAGS = -O0  -g -lrt -std=c++11

OBJ_DIR = ./obj_dir

VPATH = ./	\
		./main	\
		./util \
		./thrift_gen/calculate\
		./thrift_gen/test_service\
		${OBJ_DIR}

main_obj = main.o 
util_obj = didiutils.o
test_service_obj = test_service_constants.o TestService.o  TestServiceServer.o test_service_types.o
calculate_obj = calculate_constants.o calculate_types.o CalculatorService.o

all: client

client: ${main_obj} ${util_obj} ${test_service_obj} ${calculate_obj}
	g++ ${FLAGS} $(patsubst %.o,$(OBJ_DIR)/%.o,$(^F)) $(LIB) -o ./$@ 

%.o:%.cpp 
	@mkdir -p ${OBJ_DIR}
	@echo "Compiling $< ..."
	@g++ -c ${INCLUDE} ${FLAGS} $< -o ${OBJ_DIR}/$@

clean:
	rm -rf ${OBJ_DIR}
	rm client 
