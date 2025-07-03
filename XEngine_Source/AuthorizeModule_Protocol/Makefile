CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ -I ../XEngine_DependLibrary/XEngine_Module/jsoncpp
LOADSO = -L ../XEngine_DependLibrary/XEngine_Module/jsoncpp 
LIB = -ljsoncpp -lXEngine_BaseLib
LIBEX = 
OBJECTS = Protocol_Packet.o Protocol_Parse.o pch.o

ifeq ($(RELEASE),1)
FLAGS = -c -D _RELEASE 
DEBUG = -O2
else
	ifeq ($(PLATFORM),mac)
		FLAGS = -c
		DEBUG = -g -D _DEBUG 
	else
		FLAGS = -c -lc_p
		DEBUG = -g -pg -D _DEBUG 
	endif
endif

ifeq ($(UNICODE),1)
override UNICODE := -D __UNICODE__
else
override UNICODE := -D __ANSI__
endif

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
	else
		PLATVER = -D __UBUNTU__
	endif
	FILEEXT = so
	LIBFLAG = -shared
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
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
