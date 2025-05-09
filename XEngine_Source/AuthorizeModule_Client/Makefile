CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ -I ../XEngine_Depend/XEngine_Module/jsoncpp
LOADSO = -L ../XEngine_Depend/XEngine_Module/jsoncpp
LIB = -ljsoncpp -lXEngine_BaseLib -lXEngine_Cryption -lXClient_Socket -lXClient_APIHelp -lXEngine_SystemApi
LIBEX = 
OBJECTS = AuthClient_Connector.o AuthClient_HTTPVer.o pch.o

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
