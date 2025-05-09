CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./
LIB = -lXEngine_BaseLib -lXEngine_SystemConfig
LIBEX = 
LOADSO = 
OBJECTS = Authorize_Serial.o Authorize_CDKey.o pch.o

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
	LOADBIN = -Wl,-rpath=../XEngine_BaseLib,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/../XEngine_BaseLib/
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS)  -o libAuthorizeModule_CDKey.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

Authorize_Serial.o:./Authorize_Serial/Authorize_Serial.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Authorize_Serial/Authorize_Serial.cpp 
Authorize_CDKey.o:./Authorize_CDKey/Authorize_CDKey.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Authorize_CDKey/Authorize_CDKey.cpp 
	
pch.o:pch.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) pch.cpp


InstallAll:InstallSo
InstallSo:./libAuthorizeModule_CDKey.$(FILEEXT)
	cp ./libAuthorizeModule_CDKey.$(FILEEXT) ../../XEngine_Release/libAuthorizeModule_CDKey.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(FILEEXT)                                           
