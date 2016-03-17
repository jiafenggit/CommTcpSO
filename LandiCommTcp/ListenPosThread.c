/***********************************************************************************
 * �� �� ��   : ListenPosThread.c
 * �� �� ��   : harry
 * ��������   : 2016��3��2��
 * �ļ�����   : ����תTCP�߳�Դ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ListenPosThread.h"
#include "Commu.h"
#include "MyLog.h"

/*****************************************************************************
 * �� �� ��     : ListenPosThread
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : ����POS�߳�
 * �������  : void* arg  �߳��������
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
void* ListenPosThread(void* arg)
{
	char szDataBuf[DATE_BUF_LEN] = {0};
	unsigned int iDataLen = 0;
	int iRet = 0;

	LOG("Start ListenPosThread!");

	while(1)
	{
		iDataLen = DATE_BUF_LEN;
		memset(szDataBuf, 0x00, iDataLen);
		iRet = RecvFormPos(szDataBuf, iDataLen);
		if(iRet >= 0)
		{
			iDataLen = iRet;
			LOG("RecvFormPos succeed!len = %d", iDataLen);
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

		iRet = SendToServer(szDataBuf, iDataLen);
		if(iRet <= 0)
		{
			break;
		}
	}

	LOG("End ListenPosThread!");
}


