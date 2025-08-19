CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./
LOADSO = -L 
LIB = -lXEngine_BaseLib
LIBEX = 
OBJECTS = AuthHelp_DynamicCode.o AuthHelp_MultiLogin.o AuthHelp_APIHelp.o pch.o

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
	$(CC) $(DEBUG) $(OBJECTS) -o libAuthorizeModule_Help.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

AuthHelp_DynamicCode.o:./AuthHelp_DynamicCode/AuthHelp_DynamicCode.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthHelp_DynamicCode/AuthHelp_DynamicCode.cpp
AuthHelp_MultiLogin.o:./AuthHelp_MultiLogin/AuthHelp_MultiLogin.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthHelp_MultiLogin/AuthHelp_MultiLogin.cpp
AuthHelp_APIHelp.o:./AuthHelp_APIHelp/AuthHelp_APIHelp.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthHelp_APIHelp/AuthHelp_APIHelp.cpp

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
