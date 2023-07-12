GC=g++

all:libmyfilter.so

libmyfilter.so:config.cpp file_filter.cpp logger.cpp
	$(GC) -Wall -fPIC -shared -o libmyfilter.so config.cpp file_filter.cpp logger.cpp -ldl -lpthread
	
clean:
	rm -rf *.so