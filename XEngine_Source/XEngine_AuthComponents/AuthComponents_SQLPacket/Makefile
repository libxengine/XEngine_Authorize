CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L ../../XEngine_Lib/XEngine_BaseLib -L ../../XEngine_HelpComponents/HelpComponents_DataBase -L ../XEngine_AuthGenerateSerial
LIB = -lXEngine_BaseLib -lHelpComponents_DataBase -lXEngine_AuthGenerateSerial
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=../XEngine_BaseLib:../XEngine_HelpComponents:./,--disable-new-dtags
LIBINCLUDE = AuthRegService_SQL.o AuthRegService_Net.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libXEngine_AuthRegService.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

AuthRegService_SQL.o:./AuthRegService_SQL/AuthRegService_SQL.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./AuthRegService_SQL/AuthRegService_SQL.cpp
AuthRegService_Net.o:./AuthRegService_Net/AuthRegService_Net.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./AuthRegService_Net/AuthRegService_Net.cpp


pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp

InstallAll:InstallSo InstallHeader
InstallSo:./libXEngine_AuthRegService.so
ifeq ($(FILECENTOS), $(wildcard $(FILECENTOS)))
	cp ./libXEngine_AuthRegService.so ../../../XEngine_Release/XEngine_Linux/Centos/XEngine_AuthorizeReg/libXEngine_AuthRegService.so
else
	cp ./libXEngine_AuthRegService.so ../../../XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AuthorizeReg/libXEngine_AuthRegService.so
endif
InstallHeader:./AuthRegService_Define.h ./AuthRegService_Error.h
	cp ./AuthRegService_Define.h ../../../XEngine_Release/XEngine_Include/XEngine_AuthorizeReg/AuthRegService_Define.h
	cp ./AuthRegService_Error.h ../../../XEngine_Release/XEngine_Include/XEngine_AuthorizeReg/AuthRegService_Error.h


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.so                                           
