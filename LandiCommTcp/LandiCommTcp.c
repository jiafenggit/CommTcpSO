/***********************************************************************************
 * 文 件 名   : LandiCommTcp.c
 * 负 责 人   : harry
 * 创建日期   : 2016年3月2日
 * 文件描述   : 串口和TCP互转动态库入口源文件
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
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
 * 函 数 名     : CommTcpMap
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
int CommTcpMap(int fdComm, int fdTcp)
{
	int iRet = 0;
	pthread_t tCommToTcpID, tTcpToCommID;

	//初始化通讯参数
	initCommPara(fdComm, fdTcp);

	//创建线程
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

	//等待线程执行完成
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


