CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = 
LIB = -ljsoncpp
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = Protocol_Packet.o Protocol_Parse.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libAuthorizeModule_Protocol.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Protocol_Packet.o:./Protocol_Packet/Protocol_Packet.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Protocol_Packet/Protocol_Packet.cpp
Protocol_Parse.o:./Protocol_Parse/Protocol_Parse.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Protocol_Parse/Protocol_Parse.cpp
	
pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp

InstallAll:InstallSo
InstallSo:./libAuthorizeModule_Protocol.so
	cp ./libAuthorizeModule_Protocol.so ../../XEngine_Release/libAuthorizeModule_Protocol.so



CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.so                                           
