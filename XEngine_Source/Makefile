RELEASE = 0
UNICODE = 0
PLATFORM=linux
BUILDTYPE=0
FLAGS= 

DEPEND_JSONCPP_PATH = ./XEngine_DependLibrary/XEngine_Module/jsoncpp
DEPEND_REPORT_PATH = ./XEngine_DependLibrary/XEngine_Module/XEngine_InfoReport
DEPEND_VERIFICATION_PATH = ./XEngine_DependLibrary/XEngine_Module/XEngine_Verification

AUTH_SESSION_PATH = ./AuthorizeModule_Session
AUTH_DATABASE_PATH = ./AuthorizeModule_Database
AUTH_PROTOCOL_PATH = ./AuthorizeModule_Protocol
AUTH_CONFIGURE_PATH = ./AuthorizeModule_Configure
AUTH_HELP_PATH = ./AuthorizeModule_Help

AUTH_APP_PATH = ./XEngine_APPService/XEngine_AuthorizeService

ifeq ($(PLATFORM),linux)
	FILEEXT = so
else ifeq ($(PLATFORM),mac)
	FILEEXT = dylib
endif

# 第三方库模块
MODULES_THIRD = libjsoncpp.so libXEngine_InfoReport.so libXEngine_Verification.so                                                                                \
# 业务模块 + 应用
MODULES_APP = libAuthorizeModule_Session.so libAuthorizeModule_Database.so libAuthorizeModule_Protocol.so libAuthorizeModule_Configure.so libAuthorizeModule_Help.so   \
    		XEngine_AuthorizeApp.exe 

# 根据 BUILDTYPE 决定编译目标
ifeq ($(BUILDTYPE),0)
	XENGINE_MODULES = $(MODULES_THIRD) $(MODULES_APP)
else ifeq ($(BUILDTYPE),1)
	XENGINE_MODULES = $(MODULES_THIRD)
else ifeq ($(BUILDTYPE),2)
	XENGINE_MODULES = $(MODULES_APP)
endif

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)


libjsoncpp.so:
ifeq ($(FLAGS), InstallAll)
	cp $(DEPEND_JSONCPP_PATH)/libjsoncpp.$(FILEEXT) ../XEngine_Release/
else
	make -C $(DEPEND_JSONCPP_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
endif
libXEngine_InfoReport.so:
ifeq ($(FLAGS), InstallAll)
	cp $(DEPEND_REPORT_PATH)/libXEngine_InfoReport.$(FILEEXT) ../XEngine_Release/
else
	make -C $(DEPEND_REPORT_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
endif
libXEngine_Verification.so:
ifeq ($(FLAGS), InstallAll)
	cp $(DEPEND_VERIFICATION_PATH)/libXEngine_Verification.$(FILEEXT) ../XEngine_Release/
else
	make -C $(DEPEND_VERIFICATION_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
endif

libAuthorizeModule_Session.so:
	make -C $(AUTH_SESSION_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) $(FLAGS)
libAuthorizeModule_Database.so:
	make -C $(AUTH_DATABASE_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) $(FLAGS)
libAuthorizeModule_Protocol.so:
	make -C $(AUTH_PROTOCOL_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) $(FLAGS)
libAuthorizeModule_Configure.so:
	make -C $(AUTH_CONFIGURE_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) $(FLAGS)
libAuthorizeModule_Help.so:
	make -C $(AUTH_HELP_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) $(FLAGS)

XEngine_AuthorizeApp.exe:
	make -C $(AUTH_APP_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) $(FLAGS)