CC = g++ -Wall -std=c++17
PLATFORM = linux
PLATVER = 
PLATDIR = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./
LOADSO = -L ../../XEngine_Depend/XEngine_Module/jsoncpp -L ../../XEngine_Depend/XEngine_Module/XEngine_InfoReport  \
         -L ../../AuthorizeModule_Session -L ../../AuthorizeModule_Database -L ../../AuthorizeModule_Protocol -L ../../AuthorizeModule_Configure -L ../../AuthorizeModule_Help -L ../../AuthorizeModule_CDKey 
LIB = -lXEngine_InfoReport -ljsoncpp   \
      -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_OPenSsl -lXClient_APIHelp -lHelpComponents_Packets -lHelpComponents_DataBase -lHelpComponents_XLog -lRfcComponents_WSProtocol -lRfcComponents_HttpProtocol -lXEngine_ProcFile -lXEngine_SystemApi  \
      -lAuthorizeModule_Session -lAuthorizeModule_Database -lAuthorizeModule_Protocol -lAuthorizeModule_Configure -lAuthorizeModule_Help -lAuthorizeModule_CDKey
LIBEX = 
OBJECTS = Authorize_Config.o Authorize_Net.o Authorize_Task.o Authorize_TCPTask.o Authorize_WSTask.o Authorize_HttpTask.o XEngine_AuthorizeService.o \
          AuthorizeHTTP_Client.o AuthorizeHTTP_Serial.o AuthorizeHTTP_User.o AuthorizeHTTP_Pass.o AuthorizeHTTP_Switch.o AuthorizeHTTP_Banned.o AuthorizeHTTP_CDKey.o AuthorizeHTTP_Announcement.o AuthorizeHTTP_Try.o\
          AuthorizeHTTP_Token.o AuthorizeHTTP_GetTask.o

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
	$(CC) $(DEBUG) $(OBJECTS) -o XEngine_AuthorizeService $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)
  
Authorize_Config.o:./Authorize_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Authorize_Config.cpp 
Authorize_Net.o:./Authorize_Net.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Authorize_Net.cpp 
Authorize_Task.o:./Authorize_Task.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Authorize_Task.cpp
Authorize_TCPTask.o:./Authorize_TCPTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Authorize_TCPTask.cpp
Authorize_WSTask.o:./Authorize_WSTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Authorize_WSTask.cpp
Authorize_HttpTask.o:./Authorize_HttpTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Authorize_HttpTask.cpp

AuthorizeHTTP_Client.o:./AuthorizeHTTP_Post/AuthorizeHTTP_Client.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Post/AuthorizeHTTP_Client.cpp
AuthorizeHTTP_Serial.o:./AuthorizeHTTP_Post/AuthorizeHTTP_Serial.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Post/AuthorizeHTTP_Serial.cpp
AuthorizeHTTP_User.o:./AuthorizeHTTP_Post/AuthorizeHTTP_User.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Post/AuthorizeHTTP_User.cpp
AuthorizeHTTP_Pass.o:./AuthorizeHTTP_Post/AuthorizeHTTP_Pass.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Post/AuthorizeHTTP_Pass.cpp
AuthorizeHTTP_Switch.o:./AuthorizeHTTP_Post/AuthorizeHTTP_Switch.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Post/AuthorizeHTTP_Switch.cpp
AuthorizeHTTP_Banned.o:./AuthorizeHTTP_Post/AuthorizeHTTP_Banned.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Post/AuthorizeHTTP_Banned.cpp
AuthorizeHTTP_CDKey.o:./AuthorizeHTTP_Post/AuthorizeHTTP_CDKey.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Post/AuthorizeHTTP_CDKey.cpp
AuthorizeHTTP_Announcement.o:./AuthorizeHTTP_Post/AuthorizeHTTP_Announcement.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Post/AuthorizeHTTP_Announcement.cpp
AuthorizeHTTP_Try.o:./AuthorizeHTTP_Post/AuthorizeHTTP_Try.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Post/AuthorizeHTTP_Try.cpp

AuthorizeHTTP_Token.o:./AuthorizeHTTP_Get/AuthorizeHTTP_Token.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Get/AuthorizeHTTP_Token.cpp
AuthorizeHTTP_GetTask.o:./AuthorizeHTTP_Get/AuthorizeHTTP_GetTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./AuthorizeHTTP_Get/AuthorizeHTTP_GetTask.cpp

XEngine_AuthorizeService.o:./XEngine_AuthorizeService.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_AuthorizeService.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_AuthorizeService
	cp -f ./XEngine_AuthorizeService ../../../XEngine_Release/XEngine_AuthorizeService
	
CleanAll:CleanObj CleanMk 
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_AuthorizeService
