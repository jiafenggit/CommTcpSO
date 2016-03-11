/***********************************************************************************
 * �� �� ��   : LandiCommTcp.c
 * �� �� ��   : harry
 * ��������   : 2016��3��2��
 * �ļ�����   : ���ں�TCP��ת��̬�����Դ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
 ����ִ�����:
 gcc LandiCommTcp.c Commu.c CommToTcpThread.c TcpToCommThread.c MyLog.c -lpthread -fPIC -shared -o libLandiCommTcp.so
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "LandiCommTcp.h"
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
int StartCommTcpMap(int fdPOS, int iPosCommuType, int fdServer, char *szSoPath)
{
    int iRet = 0;
    pthread_t tCommToTcpID, tTcpToCommID;
    void *tRet = NULL;

    printf("StartCommTcpMap\n");

    //����־�ļ�
    OpenMyLog(szSoPath);

    //��ʼ��ͨѶ����
    if((iRet = initCommPara(fdPOS, iPosCommuType, fdServer)) != 0)
    {
        LOG("initCommPara failed!");
        return iRet;
    }

    //�����߳�
    iRet = pthread_create(&tCommToTcpID, NULL, CommToTcpThread, NULL);
    if(iRet != 0)
    {
        LOG("pthread_create failed!");
        return iRet;
    }

    iRet = pthread_create(&tTcpToCommID, NULL, TcpToCommThread, NULL);
    if(iRet != 0)
    {
        LOG("pthread_create failed!");
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
        LOG("CloseComm failed!");
    }

    CloseMyLog();

    return iRet;
}

