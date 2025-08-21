#include "pch.h"
#include "AuthClient_Connector.h"
/********************************************************************
//    Created:     2023/11/15  10:36:04
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_Connector\AuthClient_Connector.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_Connector
//    File Base:   AuthClient_Connector
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     验证客户端示例模块
//    History:
*********************************************************************/
CAuthClient_Connector::CAuthClient_Connector()
{
}
CAuthClient_Connector::~CAuthClient_Connector()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AuthClient_Connector_Connect
函数功能：链接到服务器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器地址
 参数.二：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：服务器端口
 参数.三：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器密码，如果没有填空
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthClient_Connector::AuthClient_Connector_Connect(LPCXSTR lpszClientAddr, int nPort, LPCXSTR lpszPass /* = NULL */)
{
	AuthClient_IsErrorOccur = true;

	if (NULL == lpszClientAddr)
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_CLIENT_PARAMENT;
		return false;
	}
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_TCP)
	if (!XClient_TCPSelect_Create(&m_hSocket, lpszClientAddr, nPort, 2))
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = XClient_GetLastError();
		return false;
	}
	if (NULL != lpszPass)
	{
		_tcsxcpy(tszPassStr, lpszPass);
	}
#endif
	return true;
}
/********************************************************************
函数名称：AuthClient_Connector_Close
函数功能：销毁关闭客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthClient_Connector::AuthClient_Connector_Close()
{
	AuthClient_IsErrorOccur = false;

#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_TCP)
	if (NULL != pSTDThread)
	{
		m_bRun = false;
		pSTDThread->join();
	}
	m_bLogin = false;
	m_bAuth = false;
	m_bHeart = false;
	XClient_TCPSelect_Close(m_hSocket);
#endif
	return true;
}
/********************************************************************
函数名称：AuthClient_Connector_GetAuth
函数功能：验证用户是否登录或者超时
 参数.一：pbAuth
  In/Out：Out
  类型：逻辑型指针
  可空：Y
  意思：输出是否验证,如果登录成功但是参数为假.说明没有剩余时间了
返回值
  类型：逻辑型
  意思：是否登录,如果没有登录将返回假,登录成功才需要判断是否通过验证
备注：
*********************************************************************/
bool CAuthClient_Connector::AuthClient_Connector_GetAuth(bool* pbAuth /* = NULL */)
{
	AuthClient_IsErrorOccur = false;
	
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_TCP)
	if (NULL != pbAuth)
	{
		*pbAuth = m_bAuth;
	}
#endif
	return m_bLogin;
}
/********************************************************************
函数名称：AuthClient_Connector_Login
函数功能：登录到服务器
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.三：lpszHWCode
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入硬件码,如果服务器开启了硬件绑定
 参数.四：nDYCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入动态码
 参数.五：xhToken
  In/Out：In
  类型：句柄型
  可空：Y
  意思：输入动态码绑定的句柄
 参数.六：dwCryption
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入密码加密类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthClient_Connector::AuthClient_Connector_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszHWCode /* = NULL */, XSHOT nDYCode /* = 0 */, XNETHANDLE xhToken /* = 0 */, XLONG dwCryption /* = 0 */)
{
	AuthClient_IsErrorOccur = false;

	if ((NULL == lpszUser) || (NULL == lpszPass))
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_CLIENT_PARAMENT;
		return false;
	}
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_TCP)
	XCHAR tszMsgBuffer[2048] = {};
	XENGINE_PROTOCOLHDR st_ProtocolHdr = {};                   
	AUTHORIZE_PROTOCOL_USERAUTHEX st_AuthUser = {};
	//协议头
	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN;
	st_ProtocolHdr.unPacketSize = sizeof(AUTHORIZE_PROTOCOL_USERAUTHEX);
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

#ifdef _MSC_BUILD
	st_AuthUser.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_WINDOWS;
#elif __linux__
	st_AuthUser.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_LINUX;
#else
	st_AuthUser.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_MACOS;
#endif
	_tcsxcpy(st_AuthUser.tszUserName, lpszUser);
	if (NULL != lpszHWCode)
	{
		_tcsxcpy(st_AuthUser.tszHWCode, lpszHWCode);
	}
	
	if (dwCryption > 0)
	{
		int nPLen = _tcsxlen(lpszPass);
		XBYTE byMD5Buffer[XPATH_MAX] = {};
		Cryption_Api_Digest(lpszPass, byMD5Buffer, &nPLen, false, dwCryption);
		BaseLib_String_StrToHex((LPCXSTR)byMD5Buffer, nPLen, st_AuthUser.tszUserPass);
	}
	else
	{
		_tcsxcpy(st_AuthUser.tszUserPass, lpszPass);
	}
	//是否有动态码
	if (nDYCode > 0)
	{
		st_ProtocolHdr.xhToken = xhToken;
		_xstprintf(st_AuthUser.tszDCode, _X("%d"), nDYCode);
	}
	//是否加密
	int nMsgLen = 0;
	if (_tcsxlen(tszPassStr) > 0)
	{
		XCHAR tszCodecBuffer[2048] = {};

		st_ProtocolHdr.wCrypto = ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT;
		Cryption_XCrypto_Encoder((LPCXSTR)&st_AuthUser, (int*)&st_ProtocolHdr.unPacketSize, (XBYTE*)tszCodecBuffer, tszPassStr);

		memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), tszCodecBuffer, st_ProtocolHdr.unPacketSize);

		nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	}
	else
	{
		memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_AuthUser, st_ProtocolHdr.unPacketSize);

		nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(AUTHORIZE_PROTOCOL_USERAUTHEX);
	}
	//发送数据
	if (!XClient_TCPSelect_SendMsg(m_hSocket, tszMsgBuffer, nMsgLen))
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_CLIENT_SEND;
		return false;
	}

	nMsgLen = 0;
	XCHAR* ptszMsgBuffer;
	st_ProtocolHdr = {};
	//接受数据
	if (!XClient_TCPSelect_RecvPkt(m_hSocket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_CLIENT_RECV;
		return false;
	}
	//判断是否登录协议
	if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN != st_ProtocolHdr.unOperatorCode)
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_CLIENT_LOGIN;
		return false;
	}
	//登录失败，错误码
	if (0 != st_ProtocolHdr.wReserve)
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = st_ProtocolHdr.wReserve;
		return false;
	}
	m_bRun = true;
	m_bLogin = true;
	m_bAuth = true;
	//登录成功，创建线程
	pSTDThread = make_shared<thread>(AuthClient_Connector_Thread, this);
	if (NULL == pSTDThread)
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_CLIENT_THREAD;
		return false;
	}
#endif
	return true;
}
/********************************************************************
函数名称：AuthClient_Connector_Heart
函数功能：启用禁用客户端心跳
 参数.一：bEnable
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是启用还是禁用
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthClient_Connector::AuthClient_Connector_Heart(bool bEnable /* = true */)
{
	AuthClient_IsErrorOccur = false;

	if (!m_bAuth)
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_CLIENT_NOTAUTH;
		return false;
	}
	m_bHeart = bEnable;
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD XCALLBACK CAuthClient_Connector::AuthClient_Connector_Thread(XPVOID lParam)
{
	CAuthClient_Connector* pClass_This = (CAuthClient_Connector*)lParam;

	time_t nTimeStart = time(NULL);

#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_TCP)
	while (pClass_This->m_bRun)
	{
		int nMsgLen = 0;
		XCHAR* ptszMsgBuffer;
		XENGINE_PROTOCOLHDR st_ProtocolHdr = {};

		if (!XClient_TCPSelect_RecvPkt(pClass_This->m_hSocket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			pClass_This->m_bRun = false;
			pClass_This->m_bLogin = false;
			pClass_This->m_bAuth = false;
			break;
		}
		XCHAR tszMsgBuffer[4096] = {};
		if (nMsgLen > 0 && _tcsxlen(pClass_This->tszPassStr) > 0)
		{
			//只有有后续数据的情况才需要解密
			Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, pClass_This->tszPassStr);
		}
		else
		{
			memcpy(tszMsgBuffer, ptszMsgBuffer, nMsgLen);
		}

		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT == st_ProtocolHdr.unOperatorCode)
		{
			pClass_This->m_bAuth = false;
		}
		//心跳支持
		if (pClass_This->m_bHeart)
		{
			time_t nTimeEnd = time(NULL);
			if ((nTimeEnd - nTimeStart) > 2)
			{
				XENGINE_PROTOCOLHDR st_ProtocolHdr = {};
				st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
				st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_HEARTBEAT;
				st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_HB_SYN;
				st_ProtocolHdr.byVersion = 1;
				st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

				nTimeStart = nTimeEnd;
				XClient_TCPSelect_SendMsg(pClass_This->m_hSocket, (LPCXSTR)&st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
#endif
	return 0;
}