CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
PLATDIR = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADSO = -L ../XEngine_ThirdPart/jsoncpp -L /usr/local/lib/XEngine_Release/XEngine_BaseLib 
LIB = -ljsoncpp -lXEngine_BaseLib
LIBEX = 
OBJECTS = Protocol_Packet.o Protocol_Parse.o pch.o

ifeq ($(RELEASE),1)
FLAGS = -c 
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

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
		PLATDIR = XEngine_Linux/Centos
	else
		PLATVER = -D __UBUNTU__
		PLATDIR = XEngine_Linux/Ubuntu
	endif
	FILEEXT = so
	LIBFLAG = -shared
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	PLATDIR = XEngine_Mac
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libAuthorizeModule_Protocol.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

Protocol_Packet.o:./Protocol_Packet/Protocol_Packet.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Protocol_Packet/Protocol_Packet.cpp
Protocol_Parse.o:./Protocol_Parse/Protocol_Parse.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Protocol_Parse/Protocol_Parse.cpp
	
pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) pch.cpp

InstallAll:InstallSo
InstallSo:./libAuthorizeModule_Protocol.$(FILEEXT)
	cp ./libAuthorizeModule_Protocol.$(FILEEXT) ../../XEngine_Release/libAuthorizeModule_Protocol.$(FILEEXT)



CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(FILEEXT)                                           
