/***********************************************************************************
 * �� �� ��   : Commu.c
 * �� �� ��   : harry
 * ��������   : 2016��3��2��
 * �ļ�����   : ͨѶ��ز���Դ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "Commu.h"
#include "LandiCommTcp.h"
#include "MyLog.h"
#include "DataOp.h"

//ͨѶ����ȫ�ֱ���
static CommPara s_CommPara;

/*****************************************************************************
 * �� �� ��     : initCommPara
 * �� �� ��     : harry
 * ��������  : 2016��3��2��
 * ��������  : ��ʼ��ͨѶ����
 * �������  : int fdComm  ���ھ��
               int fdSocket   Socket���
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int initCommPara(CommPara mCommPara)
{
	memcpy(&s_CommPara, &mCommPara, sizeof(CommPara));

	if(POS_COMMU_TYPE_COMM== s_CommPara.iPosCommuType)
	{
		//�жϴ��ڵ�״̬�Ƿ�Ϊ����״̬
		if(fcntl(s_CommPara.fdPos, F_SETFL, 0) < 0)
		{
			LOG("fcntl failed!");
			return -1;
		}
		else
		{
			LOG("fcntl=%d", fcntl(s_CommPara.fdPos, F_SETFL,0));
		}
	}

	return 0;
}

/*****************************************************************************
 * �� �� ��     : SendToPos
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : �������ݸ�POS
 * �������  : char *pDataBuf         ���ݻ���
               unsigned int iDataLen  ���ݳ���
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int SendToPos(char *pDataBuf, unsigned int iDataLen)
{
	int iLen = 0;

	if(NULL == pDataBuf)
	{
		return -1;
	}

//	LOG("start SendToPos!");

	if(POS_COMMU_TYPE_COMM == s_CommPara.iPosCommuType)
	{
		iLen = write(s_CommPara.fdPos, pDataBuf, iDataLen);
	}
	else if(POS_COMMU_TYPE_TCP == s_CommPara.iPosCommuType)
	{
		iLen = send(s_CommPara.fdPos, pDataBuf, iDataLen, 0);
	}
	else
	{
		return -2;
	}

	if (iLen == iDataLen)
	{
//		LOG("SendToPos succeed!");
		return iLen;
	}
	else
	{
		tcflush(s_CommPara.fdPos, TCOFLUSH);
		LOG("SendToPos failed");
		return -3;
	}

	return 0;
}

/*****************************************************************************
 * �� �� ��     : RecvFormPos
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : ����POS����
 * �������  : char *pDataBuf         ���ݻ���
               unsigned int iDataLen  ���ݳ���
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int RecvFormPos(char *pDataBuf, unsigned int iDataLen)
{
	int iRet = 0;
	unsigned int iLen = 0, iRecvLen = 0;
	char szTmpBuf[8] = {0};
	unsigned int iTime = 0;

	if(NULL == pDataBuf)
	{
		return COMM_RET_ERROR;
	}

//	LOG("start RecvFormPos!");

	iTime = s_CommPara.iTimeOut;
	while(iTime > 0)
	{
		if(POS_COMMU_TYPE_COMM == s_CommPara.iPosCommuType)
		{
			iRet = read(s_CommPara.fdPos, pDataBuf, iDataLen);
		}
		else if(POS_COMMU_TYPE_TCP == s_CommPara.iPosCommuType)
		{
			iRet = recv(s_CommPara.fdPos, pDataBuf, iDataLen, 0);
		}
		else
		{
			LOG("iPosCommuType is error!");
			return COMM_RET_ERROR;
		}

		if(iRet > 0)
		{
//			mLogArrayPrint(iRet, pDataBuf);
			break;
		}
		else
		{
			iTime--;
			sleep(1);

			iRet = COMM_RET_TIMEOUT;
			continue;
		}
	}

	return iRet;
}

/*****************************************************************************
 * �� �� ��     : SendToServer
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : �������ݵ������
 * �������  : char *pDataBuf         ���ݻ���
               unsigned int iDataLen  ���ݳ���
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int SendToServer(char *pDataBuf, unsigned int iDataLen)
{
	int iLen = 0;

	if(NULL == pDataBuf)
	{
		return -1;
	}

//	LOG("start SendToServer!");

	iLen = send(s_CommPara.fdServer, pDataBuf, iDataLen, 0);

	if (iLen == iDataLen)
	{
//		LOG("SendToServer succeed!");
		return iLen;
	}
	else
	{
		LOG("SendToServer failed!");
		return -2;
	}
}

/*****************************************************************************
 * �� �� ��     : RecvFormSever
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : TCP��������
 * �������  : char *pDataBuf         ���ݻ���
               unsigned int iDataLen  ���ݳ���
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int RecvFormSever(char *pDataBuf, unsigned int iDataLen)
{
	int iRet = 0;
	unsigned int iLen = 0, iRecvLen = 0;
	unsigned int iTime = 0;

	if(NULL == pDataBuf)
	{
		return COMM_RET_ERROR;
	}

//	LOG("start RecvFormServer!");

	iTime = s_CommPara.iTimeOut;
	while(iTime > 0)
	{
		iRet = recv(s_CommPara.fdServer, pDataBuf, iDataLen, 0);

		if(iRet > 0)
		{
//			mLogArrayPrint(iRet, pDataBuf);
			break;
		}
		else
		{
			iTime--;
			sleep(1);

			iRet = COMM_RET_TIMEOUT;
			continue;
		}
	}

	return iRet;
}

/*****************************************************************************
 * �� �� ��     : CloseComm
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : �ر�ͨѶ����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int CloseComm(void)
{
	int iRet = 0;

	LOG("CloseComm");

	if(POS_COMMU_TYPE_TCP == s_CommPara.iPosCommuType)
	{
		close(s_CommPara.fdPos);
	}

	iRet = close(s_CommPara.fdServer);

	return iRet;
}


