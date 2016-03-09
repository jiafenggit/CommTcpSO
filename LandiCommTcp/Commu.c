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
#include "MyLog.h"

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
int initCommPara(int fdComm, int fdSocket)
{
	s_CommPara.fdComm = fdComm;
	s_CommPara.fdSocket = fdSocket;

	//判断串口的状态是否为阻塞状态
	if(fcntl(fdComm, F_SETFL, 0) < 0)
	{
		LOG("fcntl failed!");
		return -1;
	}
	else
	{
		LOG("fcntl=%d", fcntl(fdComm, F_SETFL,0));
	}

	//设置服务器侦听队列的长度
	if(listen(s_CommPara.fdSocket, 5) <0)
	{
		LOG("listen tcp server failed!");
		return -1;
	}

	return 0;
}

/*****************************************************************************
 * 函 数 名     : CommSend
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : 串口发送数据
 * 输入参数  : char *pDataBuf         数据缓存
               unsigned int iDataLen  数据长度
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int CommSend(char *pDataBuf, unsigned int iDataLen)
{
	int len = 0;

	len = write(s_CommPara.fdComm, pDataBuf, iDataLen);
	if (len == iDataLen)
	{
		return len;
	}
	else
	{
		tcflush(s_CommPara.fdComm, TCOFLUSH);
		LOG("comm write failed");
		return -1;
	}
}

/*****************************************************************************
 * 函 数 名     : CommRecv
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : 串口接收数据
 * 输入参数  : char *pDataBuf         数据缓存
               unsigned int iDataLen  数据长度
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int CommRecv(char *pDataBuf, unsigned int iDataLen)
{
	int iRet = 0;
	int iLen = 0;
	char szTmpBuf[8] = {0};

	iLen = read(s_CommPara.fdComm, szTmpBuf, iLen);
	if(iLen <= 0)
	{
		LOG("read failed!");
	}

	return iRet;
}

/*****************************************************************************
 * 函 数 名     : TcpSend
 * 负 责 人     : harry
 * 创建日期  : 2016年3月3日
 * 函数功能  : TCP发送数据
 * 输入参数  : char *pDataBuf         数据缓存
               unsigned int iDataLen  数据长度
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int TcpSend(char *pDataBuf, unsigned int iDataLen)
{
	int iLen = 0;

	iLen = send(s_CommPara.fdSocket, pDataBuf, iDataLen, 0);

	return iLen;
}

/*****************************************************************************
 * 函 数 名     : TcpRecv
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
int TcpRecv(char *pDataBuf, unsigned int iDataLen)
{
	int iLen = 0;

	iLen = recv(s_CommPara.fdSocket, pDataBuf, iDataLen, 0);

	return iLen;
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

	iRet = close(s_CommPara.fdSocket);

	return iRet;
}


