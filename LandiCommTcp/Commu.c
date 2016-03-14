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
int initCommPara(int fdPOS, int iPosCommuType, int fdServer)
{
	s_CommPara.fdPos= fdPOS;
	s_CommPara.iPosCommuType = iPosCommuType;
	s_CommPara.fdServer= fdServer;

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
	else if(POS_COMMU_TYPE_TCP == s_CommPara.iPosCommuType)
	{
		//���÷������������еĳ���
		if(listen(s_CommPara.fdPos, 5) <0)
		{
			LOG("listen Pos failed!");
			return -2;
		}
	}
	else
	{
		return -3;
	}

	//���÷������������еĳ���
	if(listen(s_CommPara.fdServer, 5) <0)
	{
		LOG("listen server failed!");
		return -4;
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
		return iLen;
	}
	else
	{
		tcflush(s_CommPara.fdPos, TCOFLUSH);
		LOG("comm write failed");
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
	int iLen = 0;
	char szTmpBuf[8] = {0};

	if(NULL == pDataBuf)
	{
		return -1;
	}

	if(POS_COMMU_TYPE_COMM == s_CommPara.iPosCommuType)
	{
		iLen = read(s_CommPara.fdPos, szTmpBuf, iLen);
	}
	else if(POS_COMMU_TYPE_TCP == s_CommPara.iPosCommuType)
	{
		iLen = recv(s_CommPara.fdPos, pDataBuf, iDataLen, 0);
	}
	else
	{
		return -2;
	}

	if(iLen <= 0)
	{
		LOG("read failed!");
		iRet = -3;
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

	iLen = send(s_CommPara.fdServer, pDataBuf, iDataLen, 0);

	return iLen;
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
	int iLen = 0;

	if(NULL == pDataBuf)
	{
		return -1;
	}

	iLen = recv(s_CommPara.fdServer, pDataBuf, iDataLen, 0);

	return iLen;
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

	if(POS_COMMU_TYPE_TCP == s_CommPara.iPosCommuType)
	{
		close(s_CommPara.fdPos);
	}

	iRet = close(s_CommPara.fdServer);

	return iRet;
}


