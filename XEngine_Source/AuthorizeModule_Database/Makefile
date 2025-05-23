CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./
LOADSO = -L ../AuthorizeModule_CDKey
LIB = -lXEngine_BaseLib -lHelpComponents_DataBase -lAuthorizeModule_CDKey
LIBEX = 
OBJECTS = DBModule_SQLite.o DBModule_MySQL.o pch.o

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
	$(CC) $(DEBUG) $(OBJECTS) -o libAuthorizeModule_Database.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

DBModule_SQLite.o:./DBModule_SQLite/DBModule_SQLite.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./DBModule_SQLite/DBModule_SQLite.cpp
DBModule_MySQL.o:./DBModule_MySQL/DBModule_MySQL.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./DBModule_MySQL/DBModule_MySQL.cpp

pch.o:pch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) pch.cpp

InstallAll:InstallSo
InstallSo:./libAuthorizeModule_Database.$(FILEEXT)
	cp ./libAuthorizeModule_Database.$(FILEEXT) ../../XEngine_Release/libAuthorizeModule_Database.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                            
CleanMk:
	rm *.$(FILEEXT)                                           
