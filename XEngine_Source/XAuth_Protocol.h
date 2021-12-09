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
//                         ������Э�鶨��
//////////////////////////////////////////////////////////////////////////
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQDEL 0x2001          //ɾ���û�
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPDEL 0x2002
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQREGISTER 0x2003     //ע��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPREGISTER 0x2004
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN 0x2005        //��½
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN 0x2006
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQPAY 0x2007          //��ֵ
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPPAY 0x2008
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETPASS 0x2009      //�һ�����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETPASS 0x200A
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETTIME 0x200B      //��ȡʣ��ʱ��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETTIME 0x200C
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQTRYVER 0x200D       //��ʱ��֤Э������
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPTRYVER 0x200E
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETUSER 0x2010      //��ȡ�û���Ϣ����Э��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETUSER 0x2011
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQSETUSER 0x2012      //�����û���Ϣ����Э��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPSETUSER 0x2013
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_NOTIFYMSG 0x2FF0       //����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT 0x2FFF        //֪ͨ�ͻ���ʱ�䵽��
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
	ENUM_PROTOCOLDEVICE_TYPE enDeviceType;                        
}AUTHREG_PROTOCOL_TIME, * LPAUTHREG_PROTOCOL_TIME;
//�û���
typedef struct tag_AuthReg_UserTable
{
	XENGINE_PROTOCOL_USERINFO st_UserInfo;
	CHAR tszLeftTime[64];                                                 //ʣ������
	CHAR tszHardCode[32];                                                 //Ӳ����
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthRegSerialType;       //��ֵ������
	ENUM_PROTOCOLDEVICE_TYPE enDeviceType;                                //�豸����
}AUTHREG_USERTABLE, * LPAUTHREG_USERTABLE;
//ע�����кű�
typedef struct tag_AuthReg_SerialTable
{
	CHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //ʹ������˭
	CHAR tszSerialNumber[128];                                            //���к�
	CHAR tszMaxTime[64];                                                  //ʹ��ʱ��
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthRegSerialType;       //��ֵ������
	BOOL bIsUsed;                                                         //�Ƿ��Ѿ�ʹ��
	CHAR tszCreateTime[64];                                               //����ʱ��
}AUTHREG_SERIALTABLE, * LPAUTHREG_SERIALTABLE;
//������ʱ��֤��
typedef struct tag_AuthReg_NetVer
{
	CHAR tszVerSerial[256];                                               //��֤���к�
	CHAR tszVerData[64];                                                  //��֤ʱ��
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode;                  //��֤��ʽ
	int nTryTime;                                                         //����ʱ��
}AUTHREG_NETVER, * LPAUTHREG_NETVER;