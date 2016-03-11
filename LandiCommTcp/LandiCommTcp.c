/***********************************************************************************
 * 文 件 名   : LandiCommTcp.c
 * 负 责 人   : harry
 * 创建日期   : 2016年3月2日
 * 文件描述   : 串口和TCP互转动态库入口源文件
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
 编译执行语句:
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
 * 函 数 名     : StartCommTcpMap
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : 串口和TCP映射入口
 * 输入参数  : int fdComm  串口句柄
               int fdTcp   TCP句柄
 * 输出参数  : 无
 * 返 回 值     : 
 * 调用关系  : 
 * 其    它        : 
 * 修    改        : 

*****************************************************************************/
int StartCommTcpMap(int fdPOS, int iPosCommuType, int fdServer, char *szSoPath)
{
    int iRet = 0;
    pthread_t tCommToTcpID, tTcpToCommID;
    void *tRet = NULL;

    printf("StartCommTcpMap\n");

    //打开日志文件
    OpenMyLog(szSoPath);

    //初始化通讯参数
    if((iRet = initCommPara(fdPOS, iPosCommuType, fdServer)) != 0)
    {
        LOG("initCommPara failed!");
        return iRet;
    }

    //创建线程
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

    //等待线程执行完成
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
 * 函 数 名     : EndCommTcpMap
 * 负 责 人     : harry
 * 创建日期  : 2016年3月8日
 * 函数功能  : 结束串口TCP互转
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值     : 
 * 调用关系  : 
 * 其    它        : 
 * 修    改        : 

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

