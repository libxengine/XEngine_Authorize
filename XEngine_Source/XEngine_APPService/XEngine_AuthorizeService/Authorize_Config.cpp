#include "Authorize_Hdr.h"

bool Authorize_Service_Parament(int argc, char** argv)
{
    LPCXSTR lpszFile = _X("./XEngine_Config/XEngine_Config.json");
	LPCXSTR lpszVersionFile = _X("./XEngine_Config/XEngine_VersionConfig.json");
	LPCXSTR lpszSwitchFile = _X("./XEngine_Config/XEngine_SwitchConfig.json");

	if (!ModuleConfigure_Json_File(lpszFile, &st_AuthConfig))
	{
		printf("解析配置文件失败,Configure_IniFile_Read:%lX\n", Config_GetLastError());
		return false;
	}
	if (!ModuleConfigure_Json_Versions(lpszVersionFile, &st_AuthConfig))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_Versions:%lX\n", Config_GetLastError());
		return false;
	}
	if (!ModuleConfigure_Json_Switch(lpszSwitchFile, &st_FunSwitch))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_Switch:%lX\n", Config_GetLastError());
		return false;
	}

    for (int i = 0;i < argc;i++)
    {
		if ((0 == _tcsxcmp("-h", argv[i])) || (0 == _tcsxcmp("-H", argv[i])))
		{
			Authorize_Service_ParamentHelp();
			return false;
		}
		else if (0 == _tcsxcmp("-d", argv[i]))
		{
			st_AuthConfig.bDeamon = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-TP", argv[i]))
		{
			st_AuthConfig.nTCPPort = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-WP", argv[i]))
		{
			st_AuthConfig.nWSPort = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-HP", argv[i]))
		{
			st_AuthConfig.nHTTPPort = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-LL", argv[i]))
		{
			st_AuthConfig.st_XLog.nLogLeave = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-LT", argv[i]))
		{
			st_AuthConfig.st_XLog.nLogType = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-t", argv[i]))
		{
			bIsTest = true;
		}
		else if (0 == _tcsxcmp("-lt", argv[i]))
		{
			st_AuthConfig.st_XLog.nLogType = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-ll", argv[i]))
		{
			st_AuthConfig.st_XLog.nLogLeave = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-l", argv[i]))
		{
			LPCXSTR lpszLogLevel = argv[++i];
			if (0 == _tcsxcmp("debug", lpszLogLevel))
			{
				st_AuthConfig.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG;
			}
			else if (0 == _tcsxcmp("detail", lpszLogLevel))
			{
				st_AuthConfig.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DETAIL;
			}
			else if (0 == _tcsxcmp("info", lpszLogLevel))
			{
				st_AuthConfig.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO;
			}
		}
    }

    return true;
}

void Authorize_Service_ParamentHelp()
{
    printf(_X("--------------------------启动参数帮助开始--------------------------\n"));
    printf(_X("网络授权服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的ini配置文件里面的参数\n"));
    printf(_X("-h or -H：启动参数帮助提示信息\n"));
    printf(_X("-d：1 启用守护进程，2不启用\n"));
    printf(_X("--------------------------启动参数帮助结束--------------------------\n"));
}
