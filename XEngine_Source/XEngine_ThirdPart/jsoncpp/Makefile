CC = g++ -Wall -std=c++17 -fPIC
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = 
LIB = 
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = json_reader.o json_value.o json_writer.o

ifeq ($(OS),Windows_NT)
else
	ifeq ($(shell uname),Darwin)
		SHAREDLL = dylib
		SHAREFLAG = -dynamiclib
		LOADSO = 
	else
		SHAREDLL = so
		SHAREFLAG = -shared
	endif
endif

ifeq ($(RELEASE),1)
FLAGS = -c -O2
DEBUG =
else
FLAGS = -c
DEBUG = -g
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE =
endif

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libjsoncpp.$(SHAREDLL) $(SHAREFLAG) -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

json_reader.o:json_reader.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) json_reader.cpp
json_value.o:json_value.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) json_value.cpp
json_writer.o:json_writer.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) json_writer.cpp

InstallAll:InstallSo
InstallSo:./libjsoncpp.$(SHAREDLL)
	cp ./libjsoncpp.$(SHAREDLL) ../../../XEngine_Release/libjsoncpp.$(SHAREDLL)



CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(SHAREDLL)                                           
