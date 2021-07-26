CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents
LIB = -lXEngine_BaseLib -lHelpComponents_DataBase -lHelpComponents_Authorize
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = AuthService_SQLPacket.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libAuthComponents_SQLPacket.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

AuthService_SQLPacket.o:./AuthService_SQLPacket/AuthService_SQLPacket.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./AuthService_SQLPacket/AuthService_SQLPacket.cpp

pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp

InstallAll:InstallSo
InstallSo:./libAuthComponents_SQLPacket.so
	cp ./libAuthComponents_SQLPacket.so ../../../XEngine_Release/libAuthComponents_SQLPacket.so



CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.so                                           
