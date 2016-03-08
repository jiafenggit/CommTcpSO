/***********************************************************************************
 * �� �� ��   : TcpToCommThread.c
 * �� �� ��   : harry
 * ��������   : 2016��3��3��
 * �ļ�����   : TCPת�����߳�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
***********************************************************************************/

#include "Commu.h"
#include "MyLog.h"

/*****************************************************************************
 * �� �� ��     : TcpToCommThread
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : TCPת�����߳�
 * �������  : void* arg  �߳��������
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
void* TcpToCommThread(void* arg)
{
	char szDataBuf[DATE_BUF_LEN] = {0};
	unsigned int iDataLen = 0;
	int iRet = 0;

	while(1)
	{
		iRet = TcpRecv(szDataBuf, iDataLen);
		if(iRet <= 0)
		{
			continue;
		}
		else
		{
			iDataLen = iRet;
			iRet = 0;
		}

		iRet = CommSend(szDataBuf, iDataLen);
		if(iRet <= 0)
		{
			//TODO:log
			syslog(LOG_DEBUG, "CommSend failed! /n");
		}
	}
}


