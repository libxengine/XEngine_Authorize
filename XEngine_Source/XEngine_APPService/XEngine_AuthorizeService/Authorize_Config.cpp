#include "Authorize_Hdr.h"

XBOOL Authorize_Service_Parament(int argc, char** argv)
{
    LPCXSTR lpszFile = _T("./XEngine_Config/XEngine_Config.json");
	LPCXSTR lpszSwitchFile = _T("./XEngine_Config/XEngine_SwitchConfig.json");

	if (!ModuleConfigure_Json_File(lpszFile, &st_AuthConfig))
	{
		printf("解析配置文件失败,Configure_IniFile_Read:%lX\n", Config_GetLastError());
		return FALSE;
	}
	if (!ModuleConfigure_Json_Switch(lpszSwitchFile, &st_FunSwitch))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_Switch:%lX\n", Config_GetLastError());
		return FALSE;
	}

    for (int i = 0;i < argc;i++)
    {
		if ((0 == _tcscmp("-h", argv[i])) || (0 == _tcscmp("-H", argv[i])))
		{
			Authorize_Service_ParamentHelp();
			return FALSE;
		}
		else if (0 == _tcscmp("-d", argv[i]))
		{
			st_AuthConfig.bDeamon = _ttoi(argv[i + 1]);
		}
    }

    return TRUE;
}

void Authorize_Service_ParamentHelp()
{
    printf(_T("--------------------------启动参数帮助开始--------------------------\n"));
    printf(_T("网络授权服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的ini配置文件里面的参数\n"));
    printf(_T("-h or -H：启动参数帮助提示信息\n"));
    printf(_T("-d：1 启用守护进程，2不启用\n"));
    printf(_T("--------------------------启动参数帮助结束--------------------------\n"));
}
