/***********************************************************************************
 * �� �� ��   : CommToTcpThread.c
 * �� �� ��   : harry
 * ��������   : 2016��3��2��
 * �ļ�����   : ����תTCP�߳�Դ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
***********************************************************************************/
#include <stdio.h>

#include "CommToTcpThread.h"
#include "Commu.h"
#include "MyLog.h"

/*****************************************************************************
 * �� �� ��     : CommToTcpThread
 * �� �� ��     : harry
 * ��������  : 2016��3��3��
 * ��������  : ����תTCP�߳�
 * �������  : void* arg  �߳��������
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
void* CommToTcpThread(void* arg)
{
    char szDataBuf[DATE_BUF_LEN] = {0};
    unsigned int iDataLen = 0;
    int iRet = 0;

    while(1)
    {
        iRet = CommRecv(szDataBuf, iDataLen);
        if(iRet <= 0)
        {
            continue;
        }
        else
        {
            iDataLen = iRet;
            iRet = 0;
        }

        iRet = TcpSend(szDataBuf, iDataLen);
        if(iRet <= 0)
        {
            //TODO:log
            LOG("TcpSend failed!");
    	}
    }
}


