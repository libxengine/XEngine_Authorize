RELEASE = 0
UNICODE = 0
PLATFORM=linux
FLAGS= 

THIRD_JSONCPP_PATH = ./XEngine_ThirdPart/jsoncpp

AUTH_SESSION_PATH = ./AuthorizeModule_Session
AUTH_DATABASE_PATH = ./AuthorizeModule_Database
AUTH_PROTOCOL_PATH = ./AuthorizeModule_Protocol
AUTH_CONFIGURE_PATH = ./AuthorizeModule_Configure

AUTH_APP_PATH = ./XEngine_APPService/XEngine_AuthorizeService

XENGINE_MODULES = libjsoncpp.so                                                                                        \
          libAuthorizeModule_Session.so libAuthorizeModule_Database.so libAuthorizeModule_Protocol.so libAuthorizeModule_Configure.so     \
	      XEngine_AuthorizeApp.exe 

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)

libjsoncpp.so:
	make -C $(THIRD_JSONCPP_PATH) PLATFORM=$(PLATFORM) $(FLAGS)

libAuthorizeModule_Session.so:
	make -C $(AUTH_SESSION_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libAuthorizeModule_Database.so:
	make -C $(AUTH_DATABASE_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libAuthorizeModule_Protocol.so:
	make -C $(AUTH_PROTOCOL_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libAuthorizeModule_Configure.so:
	make -C $(AUTH_CONFIGURE_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
		
XEngine_AuthorizeApp.exe:
	make -C $(AUTH_APP_PATH) PLATFORM=$(PLATFORM) $(FLAGS)