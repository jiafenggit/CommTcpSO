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
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>

#include "Commu.h"
#include "CommToTcpThread.h"
#include "TcpToCommThread.h"


/*****************************************************************************
 * �� �� ��     : CommTcpMap
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
int CommTcpMap(int fdComm, int fdTcp)
{
	int iRet = 0;
	pthread_t tCommToTcpID, tTcpToCommID;

	//��ʼ��ͨѶ����
	initCommPara(fdComm, fdTcp);

	//�����߳�
	iRet = pthread_create(&tCommToTcpID, NULL, CommToTcpThread, NULL);
	if(iRet != 0)
	{
		return iRet;
	}
	
	iRet = pthread_create(&tTcpToCommID, NULL, TcpToCommThread, NULL);
	if(iRet != 0)
	{
		return iRet;
	}

	//�ȴ��߳�ִ�����
	while(1)
	{
		if((iRet = pthread_join()) == 0 && (iRet = pthread_join()) == 0)
		{
			break;
		}
		else
		{
		}
	}

	return iRet;
}


