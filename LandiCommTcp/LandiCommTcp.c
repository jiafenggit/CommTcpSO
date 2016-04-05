/***********************************************************************************
 * �� �� ��   : LandiCommTcp.c
 * �� �� ��   : harry
 * ��������   : 2016��3��2��
 * �ļ�����   : ���ں�TCP��ת��̬�����Դ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
 ����ִ�����:
 gcc LandiCommTcp.c Commu.c ListenPosThread.c ListenServerThread.c MyLog.c -lpthread -fPIC -shared -o libLandiCommTcp.so
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "LandiCommTcp.h"
#include "Commu.h"
#include "ListenPosThread.h"
#include "ListenServerThread.h"
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
int StartCommTcpMap(int fdPOS, int iPosCommuType, int fdServer, unsigned int iTimeOut, char *szSoPath)
{
	int iRet = 0;
	pthread_t tCommToTcpID, tTcpToCommID;
	void *tRet = NULL;

	printf("StartCommTcpMap\n");

	//����־�ļ�
	OpenMyLog(szSoPath);

	//��ʼ��ͨѶ����
	CommPara mCommPara;
	memset(&mCommPara, 0x00, sizeof(CommPara));
	mCommPara.fdPos = fdPOS;
	mCommPara.iPosCommuType = iPosCommuType;
	mCommPara.fdServer = fdServer;
	mCommPara.iTimeOut = iTimeOut;
	if((iRet = initCommPara(mCommPara)) != 0)
	{
		LOG("initCommPara failed!");
		return iRet;
	}

	//�����߳�
	iRet = pthread_create(&tCommToTcpID, NULL, ListenPosThread, NULL);
	if(iRet != 0)
	{
		LOG("pthread_create failed!");
		return iRet;
	}

	iRet = pthread_create(&tTcpToCommID, NULL, ListenServerThread, NULL);
	if(iRet != 0)
	{
		LOG("pthread_create failed!");
		return iRet;
	}

	//�ȴ��߳�ִ�����
	while(1)
	{
		if((iRet = pthread_join(tCommToTcpID, &tRet)) == 0 || (iRet = pthread_join(tTcpToCommID, &tRet)) == 0)
		{
			break;
		}
		else
		{
			iRet = -1;
			sleep(1);
		}
	}

	iRet = CloseComm();
	if(iRet <= 0)
	{
		LOG("CloseComm failed!");
	}

	CloseMyLog();

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
		LOG("CloseComm failed!");
	}

	CloseMyLog();

	return iRet;
}

