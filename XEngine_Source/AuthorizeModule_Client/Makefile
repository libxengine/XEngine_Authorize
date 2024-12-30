CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
PLATDIR = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ -I ../XEngine_Depend/XEngine_Module/jsoncpp
LOADSO = -L ../XEngine_Depend/XEngine_Module/jsoncpp
LIB = -ljsoncpp -lXEngine_BaseLib -lXEngine_Cryption -lXClient_Socket -lXClient_APIHelp -lXEngine_SystemApi
LIBEX = 
OBJECTS = AuthClient_Connector.o AuthClient_HTTPVer.o pch.o

ifeq ($(RELEASE),1)
FLAGS = -c 
DEBUG = 
else
	ifeq ($(PLATFORM),mac)
		FLAGS = -c
		DEBUG = -g 
	else
		FLAGS = -c -lc_p
		DEBUG = -g -pg 
	endif
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
	$(CC) $(DEBUG) $(OBJECTS) -o libAuthorizeModule_Client.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

AuthClient_Connector.o:./AuthClient_Connector/AuthClient_Connector.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthClient_Connector/AuthClient_Connector.cpp
AuthClient_HTTPVer.o:./AuthClient_HTTPVer/AuthClient_HTTPVer.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthClient_HTTPVer/AuthClient_HTTPVer.cpp

pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) pch.cpp

InstallAll:InstallSo 
InstallSo:./libAuthorizeModule_Client.$(FILEEXT)
	cp ./libAuthorizeModule_Client.$(FILEEXT) ../../XEngine_Release/libAuthorizeModule_Client.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(FILEEXT)                                           
