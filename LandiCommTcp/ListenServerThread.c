/***********************************************************************************
 * �� �� ��   : ListenServerThread.c
 * �� �� ��   : harry
 * ��������   : 2016��3��3��
 * �ļ�����   : TCPת�����߳�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ListenServerThread.h"
#include "Commu.h"
#include "MyLog.h"

/*****************************************************************************
 * �� �� ��     : ListenServerThread
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
void* ListenServerThread(void* arg)
{
	char szDataBuf[DATE_BUF_LEN] = {0};
	unsigned int iDataLen = 0;
	int iRet = 0;

	LOG("Start ListenServerThread!");

	while(1)
	{
		iRet = RecvFormSever(szDataBuf, iDataLen);
		if(iRet >= 0)
		{
			iDataLen = iRet;
		}
		else if(COMM_RET_ERROR == iRet)
		{
			LOG("RecvFormPos error, end ListenPosThread!");
			break;
		}
		else if(COMM_RET_TIMEOUT == iRet)
		{
			LOG("RecvFormPos timeout, end ListenPosThread!");
			break;
		}
		else
		{
			LOG("RecvFormPos return don't know result, end ListenPosThread!");
			break;
		}

		iRet = SendToPos(szDataBuf, iDataLen);
		if(iRet <= 0)
		{
			//TODO:log
			LOG("CommSend failed!");
		}
	}
}


