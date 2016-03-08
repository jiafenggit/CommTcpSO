/***********************************************************************************
 * �� �� ��   : LandiCommTcp.c
 * �� �� ��   : harry
 * ��������   : 2016��3��2��
 * �ļ�����   : ���ں�TCP��ת��̬�����Դ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "Commu.h"
#include "CommToTcpThread.h"
#include "TcpToCommThread.h"
#include "MyLog.h"


/*****************************************************************************
 * �� �� ��     : StartCommTcpMap
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : ���ں�TCPӳ�����
 * �������  : int fdComm  ���ھ��
               int fdTcp   TCP���
 * �������  : ��
 * �� �� ֵ     : 
 * ���ù�ϵ  : 
 * ��    ��        : 
 * ��    ��        : 

*****************************************************************************/
int StartCommTcpMap(int fdComm, int fdTcp)
{
	int iRet = 0;
	pthread_t tCommToTcpID, tTcpToCommID;
	void *tRet = NULL;

	//����־�ļ�
	openlog("LandiCommTcpLog", LOG_CONS | LOG_PID, 0);
	syslog(LOG_DEBUG, "Start CommTcpMap so! /n");

	//��ʼ��ͨѶ����
	if((iRet = initCommPara(fdComm, fdTcp)) != 0)
	{
		syslog(LOG_DEBUG, "initCommPara failed! /n");
		return iRet;
	}

	//�����߳�
	iRet = pthread_create(&tCommToTcpID, NULL, CommToTcpThread, NULL);
	if(iRet != 0)
	{
		syslog(LOG_DEBUG, "pthread_create failed! /n");
		return iRet;
	}
	
	iRet = pthread_create(&tTcpToCommID, NULL, TcpToCommThread, NULL);
	if(iRet != 0)
	{
		syslog(LOG_DEBUG, "pthread_create failed! /n");
		return iRet;
	}

	//�ȴ��߳�ִ�����
	while(1)
	{
		if((iRet = pthread_join(tCommToTcpID, &tRet)) == 0 && (iRet = pthread_join(tTcpToCommID, &tRet)) == 0)
		{
			break;
		}
		else
		{
		}
	}

	return iRet;
}

/*****************************************************************************
 * �� �� ��     : EndCommTcpMap
 * �� �� ��     : harry
 * ��������  : 2016��3��8��
 * ��������  : ��������TCP��ת
 * �������  : ��
 * �������  : ��
 * �� �� ֵ     : 
 * ���ù�ϵ  : 
 * ��    ��        : 
 * ��    ��        : 

*****************************************************************************/
int EndCommTcpMap(void)
{
	int iRet = 0;

	iRet = CloseComm();
	if(iRet <= 0)
	{
		syslog(LOG_DEBUG, "CloseComm failed! /n");
	}

	closelog();

	return iRet;
}

