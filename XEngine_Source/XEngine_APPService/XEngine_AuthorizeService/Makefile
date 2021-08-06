CC = g++ -Wall
RELEASE = 0
UNICODE = 0  
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L ../../XEngine_AuthComponents/AuthComponents_Session -L ../../XEngine_AuthComponents/AuthComponents_SQLPacket -L ../../AuthorizeModule_Protocol -L ../../AuthorizeModule_Configure
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_OPenSsl -lHelpComponents_Packets -lHelpComponents_DataBase -lHelpComponents_XLog -lHelpComponents_Authorize -lXEngine_ProcSdk -lXEngine_SystemApi -lAuthComponents_Session -lAuthComponents_SQLPacket -lAuthorizeModule_Protocol -lAuthorizeModule_Configure
LIBEX = -static-libgcc -ldl -lrt -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = Authorize_Config.o Authorize_Net.o Authorize_Task.o Authorize_TCPTask.o XEngine_AuthorizeService.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_AuthorizeService $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
Authorize_Config.o:./Authorize_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_Config.cpp 
Authorize_Net.o:./Authorize_Net.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_Net.cpp 
Authorize_Task.o:./Authorize_Task.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_Task.cpp
Authorize_TCPTask.o:./Authorize_TCPTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Authorize_TCPTask.cpp
				
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
