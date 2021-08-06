#include "pch.h"
#include "Configure_ini.h"
/********************************************************************
//    Created:     2021/07/26  09:59:49
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure\Configure_ini\Configure_ini.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure\Configure_ini
//    File Base:   Configure_ini
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     �����ļ���
//    History:
*********************************************************************/
CConfigure_IniFile::CConfigure_IniFile()
{

}
CConfigure_IniFile::~CConfigure_IniFile()
{

}
//////////////////////////////////////////////////////////////////////////
//                              ���к���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�Configure_IniFile_Read
�������ܣ���ȡ�����ļ���Ϣ
 ����.һ��lpszFile
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�����������ļ�λ��
 ����.����pSt_AuthConfig
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼�������ȡ����������Ϣ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CConfigure_IniFile::Configure_IniFile_Read(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE* pSt_AuthConfig)
{
	Config_IsErrorOccur = FALSE;

	if ((NULL == lpszFile) || (NULL == pSt_AuthConfig))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_AUTHORIZE_MODULE_CONFIGURE_PARAMENT;
		return FALSE;
	}
	pSt_AuthConfig->nPort = GetPrivateProfileInt(_T("ServiceConfig"), _T("ListenPort"), 0, lpszFile);
	pSt_AuthConfig->nThreads = GetPrivateProfileInt(_T("ServiceConfig"), _T("ThreadPool"), 0, lpszFile);
	pSt_AuthConfig->nVerTimeout = GetPrivateProfileInt(_T("ServiceConfig"), _T("UserVerTimed"), 0, lpszFile);
	pSt_AuthConfig->bAutoStart = GetPrivateProfileInt(_T("ServiceConfig"), _T("AutoStart"), 0, lpszFile);

	pSt_AuthConfig->st_Verification.nVerTime = GetPrivateProfileInt(_T("Verification"), _T("VerTime"), 0, lpszFile);
	pSt_AuthConfig->st_Verification.nVerMode = GetPrivateProfileInt(_T("Verification"), _T("VerMode"), 0, lpszFile);
	pSt_AuthConfig->st_Verification.nTryTime = GetPrivateProfileInt(_T("Verification"), _T("TryTime"), 0, lpszFile);
	pSt_AuthConfig->st_Verification.nTryMode = GetPrivateProfileInt(_T("Verification"), _T("TryMode"), 0, lpszFile);

	pSt_AuthConfig->st_Crypto.bEnable = GetPrivateProfileInt(_T("Crypto"), _T("Enable"), 0, lpszFile);
	pSt_AuthConfig->st_Crypto.nPassword = GetPrivateProfileInt(_T("Crypto"), _T("Pass"), 0, lpszFile);

	GetPrivateProfileString(_T("SmtpConfig"), _T("SmtpService"), NULL, pSt_AuthConfig->st_EMail.tszSmtpAddr, MAX_PATH, lpszFile);
	GetPrivateProfileString(_T("SmtpConfig"), _T("SmtpFromAddr"), NULL, pSt_AuthConfig->st_EMail.tszSmtpFrom, MAX_PATH, lpszFile);
	GetPrivateProfileString(_T("SmtpConfig"), _T("SmtpUser"), NULL, pSt_AuthConfig->st_EMail.tszSmtpUser, MAX_PATH, lpszFile);
	GetPrivateProfileString(_T("SmtpConfig"), _T("SmtpPass"), NULL, pSt_AuthConfig->st_EMail.tszSmtpPass, MAX_PATH, lpszFile);

	GetPrivateProfileString(_T("XSql"), _T("tszSQLite"), NULL, pSt_AuthConfig->st_XSql.tszSQLite, MAX_PATH, lpszFile);

	pSt_AuthConfig->st_XLog.nLogLeave = GetPrivateProfileInt(_T("XLog"), _T("nLogLeave"), 0, lpszFile);
	pSt_AuthConfig->st_XLog.nMaxCount = GetPrivateProfileInt(_T("XLog"), _T("nMaxCount"), 0, lpszFile);
	pSt_AuthConfig->st_XLog.nMaxSize = GetPrivateProfileInt(_T("XLog"), _T("nMaxSize"), 0, lpszFile);
	GetPrivateProfileString(_T("XLog"), _T("tszLogFile"), NULL, pSt_AuthConfig->st_XLog.tszLogFile, MAX_PATH, lpszFile);
	return TRUE;
}
/********************************************************************
�������ƣ�Configure_IniFile_Write
�������ܣ�д�������ļ���Ϣ
 ����.һ��lpszFile
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�����������ļ�λ��
 ����.����pSt_AuthConfig
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������д���������Ϣ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CConfigure_IniFile::Configure_IniFile_Write(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE* pSt_AuthConfig)
{
	Config_IsErrorOccur = FALSE;
	if ((NULL == lpszFile) || (NULL == pSt_AuthConfig))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_AUTHORIZE_MODULE_CONFIGURE_PARAMENT;
			return FALSE;
	}
	TCHAR tszBuffer[MAX_PATH];

	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->nPort);
	WritePrivateProfileString(_T("ServiceConfig"), _T("ListenPort"), tszBuffer, lpszFile);
	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->nThreads);
	WritePrivateProfileString(_T("ServiceConfig"), _T("ThreadPool"), tszBuffer, lpszFile);
	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->nVerTimeout);
	WritePrivateProfileString(_T("ServiceConfig"), _T("UserVerTimed"), tszBuffer, lpszFile);
	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->bAutoStart);
	WritePrivateProfileString(_T("ServiceConfig"), _T("AutoStart"), tszBuffer, lpszFile);

	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->st_Verification.nTryTime);
	WritePrivateProfileString(_T("Verification"), _T("TryTime"), tszBuffer, lpszFile);
	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->st_Verification.nTryMode);
	WritePrivateProfileString(_T("Verification"), _T("TryMode"), tszBuffer, lpszFile);
	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->st_Verification.nVerTime);
	WritePrivateProfileString(_T("Verification"), _T("VerTime"), tszBuffer, lpszFile);
	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->st_Verification.nVerMode);
	WritePrivateProfileString(_T("Verification"), _T("VerMode"), tszBuffer, lpszFile);

	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->st_Crypto.bEnable);
	WritePrivateProfileString(_T("Crypto"), _T("Enable"), tszBuffer, lpszFile);
	memset(tszBuffer, '\0', MAX_PATH);
	_stprintf(tszBuffer, _T("%d"), pSt_AuthConfig->st_Crypto.nPassword);
	WritePrivateProfileString(_T("Crypto"), _T("Pass"), tszBuffer, lpszFile);

	WritePrivateProfileString(_T("SmtpConfig"), _T("SmtpService"), pSt_AuthConfig->st_EMail.tszSmtpAddr, lpszFile);
	WritePrivateProfileString(_T("SmtpConfig"), _T("SmtpFromAddr"), pSt_AuthConfig->st_EMail.tszSmtpFrom, lpszFile);
	WritePrivateProfileString(_T("SmtpConfig"), _T("SmtpUser"), pSt_AuthConfig->st_EMail.tszSmtpUser, lpszFile);
	WritePrivateProfileString(_T("SmtpConfig"), _T("SmtpPassword"), pSt_AuthConfig->st_EMail.tszSmtpPass, lpszFile);

	return TRUE;
}