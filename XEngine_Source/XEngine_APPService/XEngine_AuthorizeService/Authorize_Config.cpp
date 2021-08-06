#include "Authorize_Hdr.h"

BOOL Authorize_Service_Parament(int argc, char** argv)
{
    LPCTSTR lpszFile = _T("./XEngine_Config/XEngine_Config.ini");

	if (!Configure_IniFile_Read(lpszFile, &st_AuthConfig))
	{
		printf("解析配置文件失败,Configure_IniFile_Read:%lX\n", Config_GetLastError());
		return FALSE;
	}

    for (int i = 0;i < argc;i++)
    {
		if ((0 == _tcscmp("-h", argv[i])) || (0 == _tcscmp("-H", argv[i])))
		{
			Authorize_Service_ParamentHelp();
			return FALSE;
		}
		if ((0 == _tcscmp("-v", argv[i])) || (0 == _tcscmp("-V", argv[i])))
		{
			printf("Version：V2.1.0\n");
			return FALSE;
		}
		else if (0 == _tcscmp("-p", argv[i]))
		{
			st_AuthConfig.nPort = _ttoi(argv[i + 1]);
		}
		else if (0 == _tcscmp("-t", argv[i]))
		{
			st_AuthConfig.nThreads = _ttoi(argv[i + 1]);
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
    printf(_T("-p：设置TCP服务端口号\n"));
    printf(_T("-t：设置线程个数\n"));
    printf(_T("-d：1 启用守护进程，2不启用\n"));
    printf(_T("--------------------------启动参数帮助结束--------------------------\n"));
}
