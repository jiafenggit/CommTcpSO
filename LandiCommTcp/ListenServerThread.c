/***********************************************************************************
 * �� �� ��   : ListenServerThread.c
 * �� �� ��   : harry
 * ��������   : 2016��3��3��
 * �ļ�����   : TCPת�����߳�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
***********************************************************************************/

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

	while(1)
	{
		iRet = RecvFormSever(szDataBuf, iDataLen);
		if(iRet <= 0)
		{
			continue;
		}
		else
		{
			iDataLen = iRet;
			iRet = 0;
		}

		iRet = SendToPos(szDataBuf, iDataLen);
		if(iRet <= 0)
		{
			//TODO:log
			LOG("CommSend failed!");
		}
	}
}


