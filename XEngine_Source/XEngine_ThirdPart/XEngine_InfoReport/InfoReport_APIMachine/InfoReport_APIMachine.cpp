#include "pch.h"
#include "InfoReport_APIMachine.h"
/********************************************************************
//    Created:     2024/04/28  15:16:08
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_ThirdPart\XEngine_InfoReport\InfoReport_APIMachine\InfoReport_APIMachine.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_ThirdPart\XEngine_InfoReport\InfoReport_APIMachine
//    File Base:   InfoReport_APIMachine
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     信息收集报告类
//    History:
*********************************************************************/
CInfoReport_APIMachine::CInfoReport_APIMachine()
{

}
CInfoReport_APIMachine::~CInfoReport_APIMachine()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：InfoReport_APIMachine_Send
函数功能：发送一条信息报告给API服务器
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
返回值
  类型：逻辑型
  意思：是否成功
备注：lpszAPIUrl = _X("http://127.0.0.1:5501/api?function=machine");
*********************************************************************/
bool CInfoReport_APIMachine::InfoReport_APIMachine_Send(LPCXSTR lpszAPIUrl)
{
	InfoReport_IsErrorOccur = false;

    if (NULL == lpszAPIUrl)
    {
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARAMENT;
        return false;
    }
	int nLen = 0;
	int nCode = 0;

	XCHAR tszAPIUrl[MAX_PATH] = {};
	XCHAR tszOSName[128] = {};
	XCHAR tszOSVersion[128] = {};
	XCHAR tszOSBuild[128] = {};
	XCHAR tszComputerName[128] = {};
	XLONG nOSArch = 0;

	XCLIENT_APIHTTP st_HTTPParam = {};
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial = {};
	
	_xstprintf(tszAPIUrl, _X("%s&params1=0"), lpszAPIUrl);

	SystemApi_System_GetSystemVer(tszOSName, tszOSVersion, tszOSBuild, &nOSArch);
	SystemApi_System_GetSysName(NULL, tszComputerName);
	SystemApi_HardWare_GetSerial(&st_SDKSerial);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::StreamWriterBuilder st_JsonBuilder;
	Json::CharReaderBuilder st_JsonReader;

	XCHAR tszMachineText[1024] = {};
	InfoReport_APIMachine_GetText(tszMachineText);

	st_JsonRoot["tszMachineName"] = tszOSName;
	st_JsonRoot["tszMachineCode"] = st_SDKSerial.tszSystemSerial;
	st_JsonRoot["tszMachineSystem"] = tszComputerName;
	st_JsonRoot["tszMachineText"] = tszMachineText;

	st_JsonBuilder["emitUTF8"] = true;

	XCHAR* ptszMsgBuffer = NULL;
	st_HTTPParam.nTimeConnect = 2000;
	if (!APIClient_Http_Request(_X("POST"), tszAPIUrl, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), &nCode, &ptszMsgBuffer, &nLen, NULL, NULL, &st_HTTPParam))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	st_JsonRoot.clear();
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonReader.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nLen, &st_JsonRoot, &st_JsonError))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARSE;
		return false;
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	if (0 != st_JsonRoot["code"].asInt())
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_CODE;
		return false;
	}

    return true;
}
/********************************************************************
函数名称：InfoReport_APIMachine_GetTime
函数功能：获取报告次数
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
 参数.二：pInt_TimeNumber
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出报告次数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CInfoReport_APIMachine::InfoReport_APIMachine_GetTime(LPCXSTR lpszAPIUrl, __int64x* pInt_TimeNumber)
{
	InfoReport_IsErrorOccur = false;

	if (NULL == lpszAPIUrl)
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARAMENT;
		return false;
	}
	int nLen = 0;
	int nCode = 0;
	XCHAR tszAPIUrl[MAX_PATH] = {};
	XCLIENT_APIHTTP st_HTTPParam = {};
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial = {};

	_xstprintf(tszAPIUrl, _X("%s&params1=2"), lpszAPIUrl);

	SystemApi_HardWare_GetSerial(&st_SDKSerial);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::StreamWriterBuilder st_JsonBuilder;
	Json::CharReaderBuilder st_JsonReader;

	st_JsonRoot["tszMachineCode"] = st_SDKSerial.tszSystemSerial;
	st_JsonBuilder["emitUTF8"] = true;

	XCHAR* ptszMsgBuffer = NULL;
	st_HTTPParam.nTimeConnect = 2000;
	if (!APIClient_Http_Request(_X("POST"), tszAPIUrl, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), &nCode, &ptszMsgBuffer, &nLen, NULL, NULL, &st_HTTPParam))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = APIClient_GetLastError();
		return false;
	}
	st_JsonRoot.clear();
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonReader.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nLen, &st_JsonRoot, &st_JsonError))
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_PARSE;
		return false;
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	if (0 != st_JsonRoot["code"].asInt())
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_CODE;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["data"];
	//老版本
	if (st_JsonObject.isArray())
	{
		InfoReport_IsErrorOccur = true;
		InfoReport_dwErrorCode = ERROR_XENGINE_THIRDPART_INFOREPORT_VERSION;
		return false;
	}
	*pInt_TimeNumber = st_JsonObject["nTimeNumber"].asInt64();
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                           保护函数
//////////////////////////////////////////////////////////////////////////
bool CInfoReport_APIMachine::InfoReport_APIMachine_GetText(XCHAR* ptszMSGBuffer)
{
	InfoReport_IsErrorOccur = false;

	XCHAR tszOSName[128] = {};
	XCHAR tszOSVersion[128] = {};
	XCHAR tszOSBuild[128] = {};
	XLONG nOSArch = 0;
	int nProcessCount = 0;
	SYSTEMAPI_CPU_INFOMATION st_CPUInfo = {};
	SYSTEMAPI_MEMORY_INFOMATION st_MemoryInfo = {};

	Json::Value st_JsonRoot;
	Json::Value st_JsonCPUObject;
	Json::Value st_JsonOSObject;
	Json::Value st_JsonMEMObject;
	Json::StreamWriterBuilder st_JsonBuilder;

	SystemApi_System_GetSystemVer(tszOSName, tszOSVersion, tszOSBuild, &nOSArch);
	SystemApi_HardWare_GetCpuInfomation(&st_CPUInfo);
	SystemApi_System_GetMemoryUsage(&st_MemoryInfo, XENGINE_SYSTEMSDK_API_SYSTEM_SIZE_MB);
	SystemApi_System_GetProcessCount(&nProcessCount);

	st_JsonCPUObject["nCPUNumber"] = st_CPUInfo.nCpuNumber;
	st_JsonCPUObject["nCPUSpeed"] = st_CPUInfo.nCpuSpeed;

	st_JsonOSObject["OSArch"] = (Json::Value::Int)nOSArch;
	st_JsonOSObject["OSVersion"] = tszOSVersion;
	st_JsonOSObject["tszOSBuild"] = tszOSBuild;

	st_JsonMEMObject["nTotal"] = (Json::Value::UInt64)st_MemoryInfo.dwMemory_Total;
	st_JsonMEMObject["nFree"] = (Json::Value::UInt64)st_MemoryInfo.dwMemory_Free;

	st_JsonRoot["nProcessCount"] = nProcessCount;
	st_JsonRoot["st_CPUObject"] = st_JsonCPUObject;
	st_JsonRoot["st_OSObject"] = st_JsonOSObject;
	st_JsonRoot["st_MEMObject"] = st_JsonMEMObject;

	_tcsxcpy(ptszMSGBuffer, st_JsonRoot.toStyledString().c_str());
	return true;
}