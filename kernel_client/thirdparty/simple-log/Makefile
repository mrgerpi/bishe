.PHONY: all test clean

CXX := g++
CXXFLAG := -g -Wall 
LDFLAGS := -lpthread

all:
	echo "make all"
	mkdir -p include
	mkdir -p lib
	$(CXX) $(CXXFLAG)-c src/simple_config.cpp -o src/simple_config.o
	$(CXX) $(CXXFLAG)-c src/simple_log.cpp -o src/simple_log.o
	ar -rcs libsimplelog.a src/*.o
	rm -rf src/*.o
	
	cp src/*.h include/
	mv libsimplelog.a lib/
clean:
	rm -rf lib
	rm -rf include
