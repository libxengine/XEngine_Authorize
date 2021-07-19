#pragma once
/********************************************************************
//    Created:     2021/07/16  09:18:44
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol
//    File Base:   Protocol_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出的定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                          导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD  Protocol_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                          打包类协议                                  */
/************************************************************************/
/********************************************************************
函数名称：Protocol_Packet_SendPkt
函数功能：发送打包函数
 参数.一：ptszMsgBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出要打包的缓冲区
 参数.二：pInt_MsgLen
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出打包大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入协议头
 参数.四：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要发送的数据
 参数.五：nMsgLen
  In/Out：In
  类型：整数型
  可空：Y
  意思：发送数据大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_SendPkt(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer = NULL, int nMsgLen = 0);