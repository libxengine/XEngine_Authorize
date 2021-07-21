#pragma once
/********************************************************************
//    Created:     2021/07/14  13:27:45
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XAuth_Protocol.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source
//    File Base:   XAuth_Protocol
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     ��֤Э��ͷ
//    History:
*********************************************************************/
#define XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME 32
//////////////////////////////////////////////////////////////////////////
//                         ���������ݽṹ
//////////////////////////////////////////////////////////////////////////
//��ֵЭ��
typedef struct
{
	CHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];          //�û�
	CHAR tszSerialNumber[128];                                           //���к�
}AUTHREG_PROTOCOL_USERPAY, * LPAUTHREG_PROTOCOL_USERPAY;
//ʱ����ϢЭ��
typedef struct
{
	CHAR tszUserAddr[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //��ַ
	CHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //�û�
	CHAR tszLeftTime[64];                                                 //��������
	__int64x nTimeLeft;                                                   //ʣ��ʱ��
	__int64x nTimeONLine;                                                 //����ʱ��
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType;
}AUTHREG_PROTOCOL_TIME, * LPAUTHREG_PROTOCOL_TIME;
//�û���
typedef struct tag_AuthReg_UserTable
{
	XENGINE_PROTOCOL_USERREG st_UserInfo;
	CHAR tszLeftTime[64];                                                 //ʣ������
	CHAR tszHardCode[32];                                                 //Ӳ����
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthRegSerialType;                 //��ֵ������
}AUTHREG_USERTABLE, * LPAUTHREG_USERTABLE;
//ע�����кű�
typedef struct tag_AuthReg_SerialTable
{
	CHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];        //ʹ������˭
	CHAR tszSerialNumber[128];                                           //���к�
	CHAR tszMaxTime[64];                                                 //ʹ��ʱ��
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthRegSerialType;                 //��ֵ������
	BOOL bIsUsed;                                                         //�Ƿ��Ѿ�ʹ��
}AUTHREG_SERIALTABLE, * LPAUTHREG_SERIALTABLE;
//������ʱ��֤��
typedef struct tag_AuthReg_NetVer
{
	CHAR tszVerSerial[256];                                               //��֤���к�
	CHAR tszVerData[64];                                                  //��֤ʱ��
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode;                            //��֤��ʽ
	int nTryTime;                                                         //����ʱ��
}AUTHREG_NETVER, * LPAUTHREG_NETVER;