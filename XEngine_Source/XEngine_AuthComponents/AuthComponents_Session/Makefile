CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib
LIB = -lXEngine_BaseLib 
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = AuthService_Session.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libAuthComponents_Session.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

AuthService_Session.o:./AuthService_Session/AuthService_Session.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./AuthService_Session/AuthService_Session.cpp

pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp

InstallAll:InstallSo 
InstallSo:./libAuthComponents_Session.so
	cp ./libAuthComponents_Session.so ../../../XEngine_Release/libAuthComponents_Session.so


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.so                                           
