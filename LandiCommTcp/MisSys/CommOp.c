/***********************************************************************************
 * 文 件 名   : CommOp.c
 * 负 责 人   : harry
 * 创建日期   : 2016年3月14日
 * 文件描述   : MIS系统通讯操作
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
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "CommOp.h"
#include "GetConfig.h"
#include "../ComOp/ComOp.h"

//通讯参数全局变量
static CommPara s_CommPara;
static int s_fdPos1 = 0;
static int s_fdPos2 = 0;
static int s_fdServer = 0;

#define SOCKET_DATA_SIZE (28*1024)

/*****************************************************************************
 * 函 数 名     : StartCommOp
 * 负 责 人     : harry
 * 创建日期  : 2016年3月14日
 * 函数功能  : 开始通讯连接操作
 * 输入参数  : int *fdPos          POS端通讯句柄指针
               int *iPosCommuType  POS端通讯类型指针
               int *fdServer       服务端通讯句柄指针
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int StartCommOp(int *fdPos, int *iPosCommuType, int *fdServer)
{
	int iRet = 0;
	int iMisCommType = 0;

	if(NULL == fdPos || NULL == iPosCommuType || NULL == fdServer)
	{
		return -1;
	}

	iMisCommType = GetConfigFileIntValue(COMMU_PARA, MIS_COMMU_TYPE, 0);
	printf("MisCommType = %d\n", iMisCommType);

	if(POS_COMMU_TYPE_TCP == iMisCommType)
	{
		iRet = SocketServer();
		if(iRet < 0)
		{
			return iRet;
		}

		iRet = SocketClient();
		if(iRet < 0)
		{
			return iRet;
		}
	}
	else if(POS_COMMU_TYPE_COMM == iMisCommType)
	{

	}
	else
	{
		printf("MisCommType error!\n");
	}

	*fdPos = s_fdPos1;
	*iPosCommuType = iMisCommType;
	*fdServer = s_fdServer;

	return 0;
}

/*****************************************************************************
 * 函 数 名     : SocketClient
 * 负 责 人     : harry
 * 创建日期  : 2016年3月14日
 * 函数功能  : 创建socket客户端
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int SocketClient(void)
{
	int len, iRet;
	struct sockaddr_in remote_addr; //服务器端网络地址结构体
	char buf[SOCKET_DATA_SIZE];  //数据传送的缓冲区
	char TmsServerIp[16] = {0};
	unsigned int TmsServerPort = 8000;

	GetConfigFileStringValue(COMMU_PARA, TMS_SERVER_IP, "127.0.0.1", TmsServerIp, sizeof(TmsServerIp));
	TmsServerPort = GetConfigFileIntValue(COMMU_PARA, TMS_SERVER_PORT, 8000);
	printf("TmsServerIP = %s, port = %d\n", TmsServerIp, TmsServerPort);

	memset(&remote_addr, 0, sizeof(remote_addr)); //数据初始化--清零
	remote_addr.sin_family = AF_INET; //设置为IP通信
	remote_addr.sin_port = htons(TmsServerPort); //TMS服务器端口号
	iRet = inet_pton(AF_INET, TmsServerIp, &remote_addr.sin_addr.s_addr);
	if(0 > iRet)
	{
		perror("error: first parameter is not a valid address family");
		return iRet;
	}
	else if(0 == iRet)
	{
		perror("char string second parameter does not contain valid ipaddress");
	}

	/*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
	if((s_fdServer = socket(PF_INET, SOCK_STREAM, 0))<0)
	{
		perror("socket failed");
		return -1;
	}

	/*将套接字绑定到服务器的网络地址上*/
	if(connect(s_fdServer, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("connect failed");
		return -2;
	}
	printf("connected to server\n");

//	len = send(client_sockfd,buf, strlen(buf), 0);
//	len = recv(client_sockfd, buf, SOCKET_DATA_SIZE, 0);

	return 0;
}

/*****************************************************************************
 * 函 数 名     : SocketServer
 * 负 责 人     : harry
 * 创建日期  : 2016年3月14日
 * 函数功能  : 创建socket服务端
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int SocketServer(void)
{
	int len;
	struct sockaddr_in my_addr;   //服务器网络地址结构体
	struct sockaddr_in remote_addr; //客户端网络地址结构体
	int sin_size;
	char buf[SOCKET_DATA_SIZE];  //数据传送的缓冲区
	unsigned int MisServerPort = 8000;

	//读取配置文件
	MisServerPort = GetConfigFileIntValue(COMMU_PARA, MIS_SERVER_PORT, 8000);
	printf("MisServerPort = %d \n", MisServerPort);

	memset(&my_addr, 0, sizeof(my_addr)); //数据初始化--清零
	my_addr.sin_family = AF_INET; //设置为IP通信
	my_addr.sin_addr.s_addr = INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
	my_addr.sin_port = htons(MisServerPort); //服务器端口号

	/*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
	if((s_fdPos2 = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		return -1;
	}

	int flags = fcntl(s_fdPos2, F_GETFL, 0);
	fcntl(s_fdPos2, F_SETFL, flags | ~O_NONBLOCK);

	/*将套接字绑定到服务器的网络地址上*/
	if (bind(s_fdPos2, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind failed");
		return -2;
	}

	/*监听连接请求--监听队列长度为5*/
	listen(s_fdPos2, 5);

	sin_size = sizeof(struct sockaddr_in);

	/*等待客户端连接请求到达*/
	if((s_fdPos1 = accept(s_fdPos2, (struct sockaddr *)&remote_addr, &sin_size)) < 0)
	{
		perror("accept failed");
		return -3;
	}
	printf("accept client %s\n", inet_ntoa(remote_addr.sin_addr));

//	len = send(client_sockfd, "Welcome to my server/n",21,0);//发送欢迎信息
//	len = recv(client_sockfd, buf, BUFSIZ, 0);

	return 0;
}


/*****************************************************************************
 * 函 数 名     : ComClient
 * 负 责 人     : harry
 * 创建日期  : 2016年8月16日
 * 函数功能  : 创建串口客户端
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int ComClient(void)
{
	unsigned int TmsServerPort = 8000;
	
	TmsServerPort = GetConfigFileIntValue(COMMU_PARA, TMS_SERVER_PORT, 8000);
	printf("TmsServerPort = %d\n", TmsServerPort);
}

/*****************************************************************************
 * 函 数 名     : ComServer
 * 负 责 人     : harry
 * 创建日期  : 2016年8月16日
 * 函数功能  : 创建串口服务端
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
int ComServer(void)
{
	unsigned int MisServerPort = 8000;
	
	MisServerPort = GetConfigFileIntValue(COMMU_PARA, MIS_SERVER_PORT, 8000);
	printf("MisServerPort = %d \n", MisServerPort);
}

/*****************************************************************************
 * 函 数 名     : CloseCommu
 * 负 责 人     : harry
 * 创建日期  : 2016年3月15日
 * 函数功能  : 关闭socket句柄
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
void CloseCommu(void)
{
	close(s_fdPos1);
	close(s_fdPos2);
	close(s_fdServer);
}

