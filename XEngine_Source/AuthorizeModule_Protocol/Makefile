CC = g++ -Wall -std=c++17 -fPIC
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./ -I ../XEngine_ThirdPart/jsoncpp
FILECENTOS = /etc/redhat-release
LOADBIN = -L ../XEngine_ThirdPart/jsoncpp
LIB = -ljsoncpp
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = Protocol_Packet.o Protocol_Parse.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libAuthorizeModule_Protocol.$(SHAREDLL) $(SHAREFLAG) -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Protocol_Packet.o:./Protocol_Packet/Protocol_Packet.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Protocol_Packet/Protocol_Packet.cpp
Protocol_Parse.o:./Protocol_Parse/Protocol_Parse.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Protocol_Parse/Protocol_Parse.cpp
	
pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) pch.cpp

InstallAll:InstallSo
InstallSo:./libAuthorizeModule_Protocol.$(SHAREDLL)
	cp ./libAuthorizeModule_Protocol.$(SHAREDLL) ../../XEngine_Release/libAuthorizeModule_Protocol.$(SHAREDLL)



CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(SHAREDLL)                                           
