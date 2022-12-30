#pragma once
/********************************************************************
//    Created:     2022/08/12  14:13:45
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help
//    File Base:   AuthHelp_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     授权帮助函数
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD AuthHelp_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                    剪贴板导出定义                                    */
/************************************************************************/
#ifdef _MSC_BUILD
/********************************************************************
函数名称：AuthHelp_ClipBoard_Set
函数功能：设置剪贴板内容
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要拷贝到剪贴板的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：dwFormat
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入格式,格式需要参考:https://docs.microsoft.com/en-us/windows/win32/dataxchg/standard-clipboard-formats
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL AuthHelp_ClipBoard_Set(LPCSTR lpszMsgBuffer, int nMsgLen, DWORD dwFormat = 1);
/********************************************************************
函数名称：AuthHelp_ClipBoard_Get
函数功能：获取剪贴板内容
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的内容
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取的内容大小
 参数.二：dwFormat
  In/Out：In
  类型：整数型
  可空：Y
  意思：要获取的内容格式
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL AuthHelp_ClipBoard_Get(CHAR * ptszMsgBuffer, int* pInt_MsgLen, DWORD dwFormat = 1);
/********************************************************************
函数名称：AuthHelp_ClipBoard_Clear
函数功能：清空剪贴板
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL AuthHelp_ClipBoard_Clear();
/************************************************************************/
/*                    窗口类导出定义                                    */
/************************************************************************/
/********************************************************************
函数名称：AuthHelp_Windows_Dithering
函数功能：窗口抖动函数
 参数.一：hWnd
  In/Out：In
  类型：窗口句柄
  可空：N
  意思：要设置的窗口
 参数.二：nDitheringDegree
  In/Out：In
  类型：整数型
  可空：Y
  意思：抖动次数，默认4此
 参数.三：nDitheringCareer
  In/Out：In
  类型：整数型
  可空：Y
  意思：抖动间隔，默认30毫秒
返回值
  类型：逻辑型
  意思：是否成功抖动
备注：仿造QQ抖动，支持抖动后播放声音或者不播放，不播放传递NULL
*********************************************************************/
extern "C" BOOL AuthHelp_Windows_Dithering(HWND hWnd, int nDitheringDegree = 4, int nDitheringCareer = 30);
/********************************************************************
函数名称：AuthHelp_Windows_CreateTooltip
函数功能：为控件句柄添加提示
 参数.一：hWnd
  In/Out：In
  类型：句柄
  可空：N
  意思：控件的句柄
 参数.二：lpszContextOfTip
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：是否添加成功
返回值
  类型：逻辑型
  意思：是否成功添加
备注：
*********************************************************************/
extern "C" BOOL AuthHelp_Windows_CreateTooltip(HWND hWnd, LPCTSTR lpszContextOfTip);
#endif