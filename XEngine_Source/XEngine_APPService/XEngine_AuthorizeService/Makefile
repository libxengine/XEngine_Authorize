CC = g++ -Wall -std=c++17
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0  
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk   \
          -L ../../AuthorizeModule_Session -L ../../AuthorizeModule_Database -L ../../AuthorizeModule_Protocol -L ../../AuthorizeModule_Configure -L ../../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_OPenSsl -lHelpComponents_Packets -lHelpComponents_DataBase -lHelpComponents_XLog -lHelpComponents_Authorize -lRfcComponents_WSProtocol -lRfcComponents_HttpServer -lXEngine_ProcFile -lXEngine_SystemApi  \
      -lAuthorizeModule_Session -lAuthorizeModule_Database -lAuthorizeModule_Protocol -lAuthorizeModule_Configure -ljsoncpp
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = Authorize_Config.o Authorize_Net.o Authorize_Task.o Authorize_TCPTask.o Authorize_WSTask.o Authorize_HttpTask.o XEngine_AuthorizeService.o \
             AuthorizeHTTP_Client.o AuthorizeHTTP_Serial.o AuthorizeHTTP_User.o \
			 AuthorizeHTTP_Token.o

ifeq ($(OS),Windows_NT)
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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_AuthorizeService $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
Authorize_Config.o:./Authorize_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_Config.cpp 
Authorize_Net.o:./Authorize_Net.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_Net.cpp 
Authorize_Task.o:./Authorize_Task.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_Task.cpp
Authorize_TCPTask.o:./Authorize_TCPTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_TCPTask.cpp
Authorize_WSTask.o:./Authorize_WSTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_WSTask.cpp
Authorize_HttpTask.o:./Authorize_HttpTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_HttpTask.cpp

AuthorizeHTTP_Client.o:./AuthorizeHTTP_Post/AuthorizeHTTP_Client.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./AuthorizeHTTP_Post/AuthorizeHTTP_Client.cpp
AuthorizeHTTP_Serial.o:./AuthorizeHTTP_Post/AuthorizeHTTP_Serial.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./AuthorizeHTTP_Post/AuthorizeHTTP_Serial.cpp
AuthorizeHTTP_User.o:./AuthorizeHTTP_Post/AuthorizeHTTP_User.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./AuthorizeHTTP_Post/AuthorizeHTTP_User.cpp

AuthorizeHTTP_Token.o:./AuthorizeHTTP_Get/AuthorizeHTTP_Token.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./AuthorizeHTTP_Get/AuthorizeHTTP_Token.cpp
	
XEngine_AuthorizeService.o:./XEngine_AuthorizeService.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_AuthorizeService.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_AuthorizeService
	cp -f ./XEngine_AuthorizeService ../../../XEngine_Release/XEngine_AuthorizeService
	
CleanAll:CleanObj CleanMk 
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_AuthorizeService
