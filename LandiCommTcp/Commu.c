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
int initCommPara(int fdComm, int fdSocket)
{
	s_CommPara.fdComm = fdComm;
	s_CommPara.fdSocket = fdSocket;

	//�жϴ��ڵ�״̬�Ƿ�Ϊ����״̬
	if(fcntl(fdComm, F_SETFL, 0) < 0)
	{
		LOG("fcntl failed!");
		return -1;
	}
	else
	{
		LOG("fcntl=%d", fcntl(fdComm, F_SETFL,0));
	}

	//���÷������������еĳ���
	if(listen(s_CommPara.fdSocket, 5) <0)
	{
		LOG("listen tcp server failed!");
		return -1;
	}

	return 0;
}

/*****************************************************************************
 * �� �� ��     : CommSend
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : ���ڷ�������
 * �������  : char *pDataBuf         ���ݻ���
               unsigned int iDataLen  ���ݳ���
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int CommSend(char *pDataBuf, unsigned int iDataLen)
{
	int len = 0;

	len = write(s_CommPara.fdComm, pDataBuf, iDataLen);
	if (len == iDataLen)
	{
		return len;
	}
	else
	{
		tcflush(s_CommPara.fdComm, TCOFLUSH);
		LOG("comm write failed");
		return -1;
	}
}

/*****************************************************************************
 * �� �� ��     : CommRecv
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : ���ڽ�������
 * �������  : char *pDataBuf         ���ݻ���
               unsigned int iDataLen  ���ݳ���
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int CommRecv(char *pDataBuf, unsigned int iDataLen)
{
	int iRet = 0;
	int iLen = 0;
	char szTmpBuf[8] = {0};

	iLen = read(s_CommPara.fdComm, szTmpBuf, iLen);
	if(iLen <= 0)
	{
		LOG("read failed!");
	}

	return iRet;
}

/*****************************************************************************
 * �� �� ��     : TcpSend
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
int TcpSend(char *pDataBuf, unsigned int iDataLen)
{
	int iLen = 0;

	iLen = send(s_CommPara.fdSocket, pDataBuf, iDataLen, 0);

	return iLen;
}

/*****************************************************************************
 * �� �� ��     : TcpRecv
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
int TcpRecv(char *pDataBuf, unsigned int iDataLen)
{
	int iLen = 0;

	iLen = recv(s_CommPara.fdSocket, pDataBuf, iDataLen, 0);

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

	iRet = close(s_CommPara.fdSocket);

	return iRet;
}


