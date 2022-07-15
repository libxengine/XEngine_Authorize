CC = g++ -Wall -std=c++17 -fPIC
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib
LIB = -lXEngine_BaseLib 
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = Configure_ini.o pch.o

ifeq ($(OS),Windows_NT)
	SHAREFLAG = 0
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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libAuthorizeModule_Configure.$(SHAREDLL) $(SHAREFLAG) -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Configure_ini.o:./Configure_ini/Configure_ini.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Configure_ini/Configure_ini.cpp
	
pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp

InstallAll:InstallSo
InstallSo:./libAuthorizeModule_Configure.$(SHAREDLL)
	cp ./libAuthorizeModule_Configure.$(SHAREDLL) ../../XEngine_Release/libAuthorizeModule_Configure.$(SHAREDLL)



CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(SHAREDLL)                                           
