GC=g++

all:libmyfiltertest.so
libmyfiltertest.so:file_test.cc
	$(GC)  -fPIC -shared -o libmyfiltertest.so file_test.cc -ldl

clean:
	rm -rf *.so