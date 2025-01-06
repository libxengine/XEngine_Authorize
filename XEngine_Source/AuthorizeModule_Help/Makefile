CC = g++ -Wall -std=c++20 -fPIC
PLATFORM = linux
PLATVER = 
PLATDIR = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./
LOADSO = -L 
LIB = -lXEngine_BaseLib
LIBEX = 
OBJECTS = AuthHelp_DynamicCode.o AuthHelp_MultiLogin.o pch.o

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
	$(CC) $(DEBUG) $(OBJECTS) -o libAuthorizeModule_Help.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

AuthHelp_DynamicCode.o:./AuthHelp_DynamicCode/AuthHelp_DynamicCode.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthHelp_DynamicCode/AuthHelp_DynamicCode.cpp
AuthHelp_MultiLogin.o:./AuthHelp_MultiLogin/AuthHelp_MultiLogin.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthHelp_MultiLogin/AuthHelp_MultiLogin.cpp

pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) pch.cpp

InstallAll:InstallSo
InstallSo:./libAuthorizeModule_Help.$(FILEEXT)
	cp ./libAuthorizeModule_Help.$(FILEEXT) ../../XEngine_Release/libAuthorizeModule_Help.$(FILEEXT)



CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(FILEEXT)                                           
