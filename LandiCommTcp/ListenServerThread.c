/***********************************************************************************
 * 文 件 名   : ListenServerThread.c
 * 负 责 人   : harry
 * 创建日期   : 2016年3月3日
 * 文件描述   : TCP转串口线程
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   :
 * 修改日志   :
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ListenServerThread.h"
#include "Commu.h"
#include "MyLog.h"

/*****************************************************************************
 * 函 数 名     : ListenServerThread
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : TCP转串口线程
 * 输入参数  : void* arg  线程输入参数
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

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
#ifdef LOG_LEVEL_DEBUG
			LOG("RecvFormSever succeed!", iDataLen);
			mLogArrayPrint(iDataLen, szDataBuf);
#endif
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
			LOG("RecvFormSever return don't know result, end ListenServerThread!");
			break;
		}

		iRet = SendToPos(szDataBuf, iDataLen);
		if(iRet <= 0)
		{
			break;
		}
		else if(iDataLen == iRet)
		{
#ifdef LOG_LEVEL_DEBUG
			LOG("SendToPos succeed!", iDataLen);
			mLogArrayPrint(iDataLen, szDataBuf);
#endif
		}
	}

	LOG("End ListenServerThread!");
}


