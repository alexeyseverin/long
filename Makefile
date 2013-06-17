# makefile
PY_INCLUDE_PATH = /usr/include/python2.7

MAIN: clevel.o cpplevel.o cpplevel_wrap.o libcpplevel.so  #test_c.o test_cpp.o 
	#gcc -o test_c test_c.o -L. -lclevel -lm -Wl,-rpath,.
	#g++  -o test_cpp test_cpp.o -L. -lclevel -lcpplevel -Wl,-rpath,. 

libcpplevel.so: libclevel.so
	g++  -shared -o libcpplevel.so cpplevel.o -L. -lclevel -Wl,-rpath,.
	g++ cpplevel_wrap.o cpplevel.o -shared -fPIC -L. -lclevel -Wl,-rpath,. -o _cpplevel.so

libclevel.so:
	gcc -shared -o libclevel.so clevel.o


#test_c.o: test_c.c libclevel.so
#	gcc -c test_c.c


clevel.o: clevel.c
	gcc -c -fPIC clevel.c

#test_cpp.o: test_cpp.cpp libclevel.so libclevel.so
#	g++ -c test_cpp.cpp



cpplevel_wrap.o: cpplevel_wrap.cxx 
	g++ -fPIC -c cpplevel.cpp cpplevel_wrap.cxx -I$(PY_INCLUDE_PATH)

cpplevel_wrap.cxx: cpplevel.i cpplevel.o
	swig -c++ -python cpplevel.i

cpplevel.o: cpplevel.cpp libclevel.so
	g++ -c -fPIC cpplevel.cpp 

clean:
	rm -f *.o
	rm -f *.pyc

cleanall: clean
	rm -f *.cxx
	rm -f *.so



