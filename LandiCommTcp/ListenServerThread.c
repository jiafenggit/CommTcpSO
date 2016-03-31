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
		iDataLen = DATE_BUF_LEN;
		memset(szDataBuf, 0x00, iDataLen);
		iRet = RecvFormSever(szDataBuf, iDataLen);
		if(iRet >= 0)
		{
			iDataLen = iRet;
			LOG("RecvFormSever succeed!", iDataLen);
			mLogArrayPrint(iDataLen, szDataBuf);
		}
		else if(COMM_RET_ERROR == iRet)
		{
			LOG("RecvFormSever error, end ListenPosThread!");
			break;
		}
		else if(COMM_RET_TIMEOUT == iRet)
		{
			LOG("RecvFormSever timeout, end ListenPosThread!");
			break;
		}
		else
		{
			LOG("RecvFormSever return don't know result, end ListenPosThread!");
			break;
		}

		iRet = SendToPos(szDataBuf, iDataLen);
		if(iRet <= 0)
		{
			break;
		}
		else if(iDataLen == iRet)
		{
//			LOG("SendToPos succeed!", iDataLen);
//			mLogArrayPrint(iDataLen, szDataBuf);
		}
	}

	LOG("End ListenServerThread!");
}


