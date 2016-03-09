/***********************************************************************************
 * 文 件 名   : CommToTcpThread.c
 * 负 责 人   : harry
 * 创建日期   : 2016年3月2日
 * 文件描述   : 串口转TCP线程源文件
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   :
 * 修改日志   :
***********************************************************************************/
#include <stdio.h>

#include "Commu.h"
#include "MyLog.h"

/*****************************************************************************
 * 函 数 名     : CommToTcpThread
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : 串口转TCP线程
 * 输入参数  : void* arg  线程输入参数
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

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


