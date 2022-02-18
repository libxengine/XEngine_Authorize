CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = 
LIB = 
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = json_reader.o json_value.o json_writer.o

ifeq ($(RELEASE),1)
FLAGS = -c -O2
DEBUG =
else
FLAGS = -c -lc_p
DEBUG = -g -pg
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE =
endif

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libjsoncpp.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

json_reader.o:json_reader.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) json_reader.cpp
json_value.o:json_value.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) json_value.cpp
json_writer.o:json_writer.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) json_writer.cpp

InstallAll:InstallSo
InstallSo:./libjsoncpp.so
	cp ./libjsoncpp.so ../../../XEngine_Release/libjsoncpp.so



CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.so                                           
