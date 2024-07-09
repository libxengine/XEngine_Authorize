RELEASE = 0
UNICODE = 0
PLATFORM=linux
FLAGS= 

DEPEND_JSONCPP_PATH = ./XEngine_Depend/XEngine_Module/jsoncpp
DEPEND_REPORT_PATH = ./XEngine_Depend/XEngine_Module/XEngine_InfoReport

AUTH_CDKEY_PATH = ./AuthorizeModule_CDKey
AUTH_SESSION_PATH = ./AuthorizeModule_Session
AUTH_DATABASE_PATH = ./AuthorizeModule_Database
AUTH_PROTOCOL_PATH = ./AuthorizeModule_Protocol
AUTH_CONFIGURE_PATH = ./AuthorizeModule_Configure
AUTH_HELP_PATH = ./AuthorizeModule_Help
AUTH_CLIENT_PATH = ./AuthorizeModule_Client

AUTH_APP_PATH = ./XEngine_APPService/XEngine_AuthorizeService

XENGINE_MODULES = libjsoncpp.so libXEngine_InfoReport.so                                                                                    \
          libAuthorizeModule_CDKey.so libAuthorizeModule_Session.so libAuthorizeModule_Database.so libAuthorizeModule_Protocol.so libAuthorizeModule_Configure.so libAuthorizeModule_Help.so libAuthorizeModule_Client.so   \
	      XEngine_AuthorizeApp.exe 

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)

libjsoncpp.so:
	make -C $(DEPEND_JSONCPP_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libXEngine_InfoReport.so:
	make -C $(DEPEND_REPORT_PATH) PLATFORM=$(PLATFORM) $(FLAGS)

libAuthorizeModule_CDKey.so:
	make -C $(AUTH_CDKEY_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libAuthorizeModule_Session.so:
	make -C $(AUTH_SESSION_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libAuthorizeModule_Database.so:
	make -C $(AUTH_DATABASE_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libAuthorizeModule_Protocol.so:
	make -C $(AUTH_PROTOCOL_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libAuthorizeModule_Configure.so:
	make -C $(AUTH_CONFIGURE_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libAuthorizeModule_Help.so:
	make -C $(AUTH_HELP_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libAuthorizeModule_Client.so:
	make -C $(AUTH_CLIENT_PATH) PLATFORM=$(PLATFORM) $(FLAGS)

XEngine_AuthorizeApp.exe:
	make -C $(AUTH_APP_PATH) PLATFORM=$(PLATFORM) $(FLAGS)