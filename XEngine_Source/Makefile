RELEASE = 0
UNICODE = 0

FLAGS = RELEASE=$(RELEASE)

COMPONENTS_SESSION_PATH = ./XEngine_AuthComponents/AuthComponents_Session
COMPONENTS_SQLPACKET_PATH = ./XEngine_AuthComponents/AuthComponents_SQLPacket

AUTH_PROTOCOL_PATH = ./AuthorizeModule_Protocol
AUTH_CONFIGURE_PATH = ./AuthorizeModule_Configure

AUTH_APP_PATH = ./XEngine_AuthorizeApp

NETENGINE_LIB = libAuthComponents_Session.so libAuthComponents_SQLPacket.so                                          \
                libAuthorizeModule_Protocol.so libAuthorizeModule_Configure.so                                       \
		 XEngine_AuthorizeApp.exe 

.PHONY:MakeAll 
MakeAll:$(NETENGINE_LIB)

libAuthComponents_Session.so:
	make -C $(COMPONENTS_SESSION_PATH) $(FLAGS)
libAuthComponents_SQLPacket.so:
	make -C $(COMPONENTS_SQLPACKET_PATH) $(FLAGS)
	
libAuthorizeModule_Protocol.so:
	make -C $(AUTH_PROTOCOL_PATH) $(FLAGS)
libAuthorizeModule_Configure.so:
	make -C $(AUTH_CONFIGURE_PATH) $(FLAGS)
		
XEngine_AuthorizeApp.exe:
	make -C $(AUTH_APP_PATH) $(FLAGS)


SetFlags_Install:
	export FLAGS=InstallAll
InstallAll:SetFlags_Install $(NETENGINE_LIB) 
 
SetFlags_Clean:
	export FLAGS=CleanAll
CleanAll:SetFlags_Clean $(NETENGINE_LIB) 
	
	
	
