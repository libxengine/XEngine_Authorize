#pragma once
/********************************************************************
//    Created:     2021/07/26  09:55:11
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure\Config_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure
//    File Base:   Config_Define
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     �����ĺ���
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                              ���������ݽṹ
//////////////////////////////////////////////////////////////////////////
typedef struct  
{
	int nPort;                                              //�������˿�
	int nThreads;                                           //�������̸߳���
	int nVerTimeout;                                        //�û���֤��ʱʱ��
	BOOL bAutoStart;                                        //������
	struct   
	{
		int nVerTime;                                       //��֤ʱ��
		int nVerMode;                                       //��֤ģʽ
		int nTryTime;                                       //��֤ʱ��
		int nTryMode;                                       //��֤ģʽ
	}st_Verification;
	struct  
	{
		BOOL bEnable;                                       //�Ƿ����ü��ܴ��� 
		int nPassword;                                      //����
	}st_Crypto;
	struct  
	{
		TCHAR tszSmtpAddr[MAX_PATH];                        //SMTP������
		TCHAR tszSmtpFrom[MAX_PATH];                        //�ظ���ַ
		TCHAR tszSmtpUser[MAX_PATH];                        //�û���
		TCHAR tszSmtpPass[MAX_PATH];                        //����
	}st_EMail;
}AUTHORIZE_CONFIGURE;
//////////////////////////////////////////////////////////////////////////
//                              �����ĺ���
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                              �����ļ���������                        */
/************************************************************************/
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
extern "C" BOOL Configure_IniFile_Read(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE* pSt_AuthConfig);
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
extern "C" BOOL Configure_IniFile_Write(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE* pSt_AuthConfig);