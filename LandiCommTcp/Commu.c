/***********************************************************************************
 * 文 件 名   : Commu.c
 * 负 责 人   : harry
 * 创建日期   : 2016年3月2日
 * 文件描述   : 通讯相关操作源文件
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   :
 * 修改日志   :
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "Commu.h"
#include "LandiCommTcp.h"
#include "MyLog.h"
#include "DataOp.h"

//通讯参数全局变量
static CommPara s_CommPara;

/*****************************************************************************
 * 函 数 名     : initCommPara
 * 负 责 人     : harry
 * 创建日期  : 2016年3月2日
 * 函数功能  : 初始化通讯参数
 * 输入参数  : int fdComm  串口句柄
               int fdSocket   Socket句柄
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int initCommPara(CommPara mCommPara)
{
	memcpy(&s_CommPara, &mCommPara, sizeof(CommPara));

	if(POS_COMMU_TYPE_COMM== s_CommPara.iPosCommuType)
	{
		//判断串口的状态是否为阻塞状态
		if(fcntl(s_CommPara.fdPos, F_SETFL, 0) < 0)
		{
			LOG("fcntl failed!");
			return -1;
		}
		else
		{
			LOG("fcntl=%d", fcntl(s_CommPara.fdPos, F_SETFL,0));
		}
	}

	return 0;
}

/*****************************************************************************
 * 函 数 名     : SendToPos
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : 发送数据给POS
 * 输入参数  : char *pDataBuf         数据缓存
               unsigned int iDataLen  数据长度
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int SendToPos(char *pDataBuf, unsigned int iDataLen)
{
	int iLen = 0;

	if(NULL == pDataBuf)
	{
		return -1;
	}

//	LOG("start SendToPos!");

	if(POS_COMMU_TYPE_COMM == s_CommPara.iPosCommuType)
	{
		iLen = write(s_CommPara.fdPos, pDataBuf, iDataLen);
	}
	else if(POS_COMMU_TYPE_TCP == s_CommPara.iPosCommuType)
	{
		iLen = send(s_CommPara.fdPos, pDataBuf, iDataLen, 0);
	}
	else
	{
		return -2;
	}

	if (iLen == iDataLen)
	{
//		LOG("SendToPos succeed!");
		return iLen;
	}
	else
	{
		tcflush(s_CommPara.fdPos, TCOFLUSH);
		LOG("SendToPos failed");
		return -3;
	}

	return 0;
}

/*****************************************************************************
 * 函 数 名     : RecvFormPos
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : 接收POS数据
 * 输入参数  : char *pDataBuf         数据缓存
               unsigned int iDataLen  数据长度
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int RecvFormPos(char *pDataBuf, unsigned int iDataLen)
{
	int iRet = 0;
	unsigned int iLen = 0, iRecvLen = 0;
	char szTmpBuf[8] = {0};
	unsigned int iTime = 0;

	if(NULL == pDataBuf)
	{
		return COMM_RET_ERROR;
	}

//	LOG("start RecvFormPos!");

	iTime = s_CommPara.iTimeOut;
	while(iTime > 0)
	{
		if(POS_COMMU_TYPE_COMM == s_CommPara.iPosCommuType)
		{
			iRet = read(s_CommPara.fdPos, pDataBuf, iDataLen);
		}
		else if(POS_COMMU_TYPE_TCP == s_CommPara.iPosCommuType)
		{
			iRet = recv(s_CommPara.fdPos, pDataBuf, iDataLen, 0);
		}
		else
		{
			LOG("iPosCommuType is error!");
			return COMM_RET_ERROR;
		}

		if(iRet > 0)
		{
//			mLogArrayPrint(iRet, pDataBuf);
			break;
		}
		else
		{
			iTime--;
			sleep(1);

			iRet = COMM_RET_TIMEOUT;
			continue;
		}
	}

	return iRet;
}

/*****************************************************************************
 * 函 数 名     : SendToServer
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : 发送数据到服务端
 * 输入参数  : char *pDataBuf         数据缓存
               unsigned int iDataLen  数据长度
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int SendToServer(char *pDataBuf, unsigned int iDataLen)
{
	int iLen = 0;

	if(NULL == pDataBuf)
	{
		return -1;
	}

//	LOG("start SendToServer!");

	iLen = send(s_CommPara.fdServer, pDataBuf, iDataLen, 0);

	if (iLen == iDataLen)
	{
//		LOG("SendToServer succeed!");
		return iLen;
	}
	else
	{
		LOG("SendToServer failed!");
		return -2;
	}
}

/*****************************************************************************
 * 函 数 名     : RecvFormSever
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : TCP接收数据
 * 输入参数  : char *pDataBuf         数据缓存
               unsigned int iDataLen  数据长度
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int RecvFormSever(char *pDataBuf, unsigned int iDataLen)
{
	int iRet = 0;
	unsigned int iLen = 0, iRecvLen = 0;
	unsigned int iTime = 0;

	if(NULL == pDataBuf)
	{
		return COMM_RET_ERROR;
	}

//	LOG("start RecvFormServer!");

	iTime = s_CommPara.iTimeOut;
	while(iTime > 0)
	{
		iRet = recv(s_CommPara.fdServer, pDataBuf, iDataLen, 0);

		if(iRet > 0)
		{
//			mLogArrayPrint(iRet, pDataBuf);
			break;
		}
		else
		{
			iTime--;
			sleep(1);

			iRet = COMM_RET_TIMEOUT;
			continue;
		}
	}

	return iRet;
}

/*****************************************************************************
 * 函 数 名     : CloseComm
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : 关闭通讯连接
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int CloseComm(void)
{
	int iRet = 0;

	LOG("CloseComm");

	if(POS_COMMU_TYPE_TCP == s_CommPara.iPosCommuType)
	{
		close(s_CommPara.fdPos);
	}

	iRet = close(s_CommPara.fdServer);

	return iRet;
}


