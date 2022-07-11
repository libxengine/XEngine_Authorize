CC = g++ -Wall -std=c++17 -fPIC
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents
LIB = -lXEngine_BaseLib -lHelpComponents_DataBase -lHelpComponents_Authorize
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = Database_SQLite.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libAuthorizeModule_Database.$(SHAREDLL) $(SHAREFLAG) -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Database_SQLite.o:./Database_SQLite/Database_SQLite.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Database_SQLite/Database_SQLite.cpp

pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp

InstallAll:InstallSo
InstallSo:./libAuthorizeModule_Database.$(SHAREDLL)
	cp ./libAuthorizeModule_Database.$(SHAREDLL) ../../XEngine_Release/libAuthorizeModule_Database.$(SHAREDLL)


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(SHAREDLL)                                           
