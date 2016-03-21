/***********************************************************************************
 * 文 件 名   : TmsServer.c
 * 负 责 人   : harry
 * 创建日期   : 2016年3月15日
 * 文件描述   : 模拟TMS服务器
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   :
 * 修改日志   :
 编译执行语句:
  gcc TmsServer.c -o TmsServer
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


#define WORD(h, l) ((h)*0x100 + l)


#define DATE_BUF_LEN_SIZE 2 //长度域的长度
#define SOCKET_DATA_SIZE (4*1024)

#define RECV_TIME_OUT 30 //30秒

//通讯操作返回结果
#define COMM_RET_SUCCESS	0
#define COMM_RET_ERROR		-1
#define COMM_RET_TIMEOUT	-2

#define START_STR "start"
#define END_STR "end"

int SendData(int iFD, char *pDataBuf, unsigned int iDataLen);
int RecvData(int iFD, char *pDataBuf, unsigned int iBufSize, unsigned int iTimeOut);

int main()
{
	int iRet = COMM_RET_SUCCESS;
	int fdTms1 = 0, fdTms2 = 0;
	int len;
	struct sockaddr_in my_addr;   //服务器网络地址结构体
	struct sockaddr_in remote_addr; //客户端网络地址结构体
	int sin_size;
	char buf[SOCKET_DATA_SIZE];  //数据传送的缓冲区
	unsigned int iTime = 0;
	unsigned int TmsServerPort = 8099;

	printf("running...\n");

	memset(&my_addr, 0, sizeof(my_addr)); //数据初始化--清零
	my_addr.sin_family = AF_INET; //设置为IP通信
	my_addr.sin_addr.s_addr = INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
	printf("TmsServerPort = %d\n", TmsServerPort);
	my_addr.sin_port = htons(TmsServerPort); //TMS服务器端口号

	/*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
	if((fdTms2 = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed\n");
		return -1;
	}

	/*将套接字绑定到服务器的网络地址上*/
	if (bind(fdTms2, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind failed\n");
		return -2;
	}

	/*监听连接请求--监听队列长度为5*/
	listen(fdTms2, 5);

	sin_size = sizeof(struct sockaddr_in);

	/*等待客户端连接请求到达*/
	if((fdTms1 = accept(fdTms2, (struct sockaddr *)&remote_addr, &sin_size)) < 0)
	{
		perror("accept failed");
		return -3;
	}
	printf("accept client %s\n", inet_ntoa(remote_addr.sin_addr));

	//接收下载请求
	memset(buf, 0x00, SOCKET_DATA_SIZE);
	iRet = RecvData(fdTms1, buf, SOCKET_DATA_SIZE, RECV_TIME_OUT);
	if(iRet < COMM_RET_SUCCESS)
	{
		printf("1.RecvData failed!\n");
		return -4;
	}
	else
	{
		len = iRet - DATE_BUF_LEN_SIZE;
	}

	if(len != strlen(START_STR))
	{
		printf("recv data len error!\n");
		close(fdTms1);
		close(fdTms2);
		return -5;
	}
	else
	{
		if(0 != strcmp(START_STR, buf + DATE_BUF_LEN_SIZE))
		{
			printf("recv wrong message!\n");
			close(fdTms1);
			close(fdTms2);
			return -6;
		}
		else
		{
			printf("recv start str.\n");
		}
	}

	//发送握手
	printf("send message 1.\n");
	memset(buf, 0x30, SOCKET_DATA_SIZE / 2);
	len = SendData(fdTms1, buf, SOCKET_DATA_SIZE / 2);

	sleep(3);

	//发送数据
	printf("send message 2.\n");
	memset(buf, 0x31, SOCKET_DATA_SIZE / 2);
	len = SendData(fdTms1, buf, SOCKET_DATA_SIZE / 2);

	//接收下载结束报文
	iRet = RecvData(fdTms1, buf, SOCKET_DATA_SIZE, RECV_TIME_OUT);
	if(iRet < COMM_RET_SUCCESS)
	{
		printf("2.RecvData failed!\n");
		return -7;
	}
	else
	{
		len = iRet - DATE_BUF_LEN_SIZE;
	}

	if(len != strlen(END_STR))
	{
		close(fdTms1);
		close(fdTms2);
		return 0;
	}
	else
	{
		if(0 != strcmp(END_STR, buf + DATE_BUF_LEN_SIZE))
		{
			close(fdTms1);
			close(fdTms2);
			return 0;
		}
		else
		{
			printf("recv end str.\n");
		}
	}

	close(fdTms1);
	close(fdTms2);

	return 0;
}


int SendData(int iFD, char *pDataBuf, unsigned int iDataLen)
{
	char SendBuf[SOCKET_DATA_SIZE] = {0};
	unsigned int iLen = 0;

	if(NULL == pDataBuf || 0 == iDataLen || iDataLen >= SOCKET_DATA_SIZE)
	{
		return -1;
	}

	SendBuf[DATE_BUF_LEN_SIZE -2] = iDataLen / 0x100;//高位
	SendBuf[DATE_BUF_LEN_SIZE -1] = iDataLen % 0x100;//低位
	iLen += DATE_BUF_LEN_SIZE;

	memcpy(SendBuf + DATE_BUF_LEN_SIZE, pDataBuf, iDataLen);
	iLen += iDataLen;

	printf("Tms send data len = %d\n", iLen);
	iLen = send(iFD, SendBuf, iLen, 0);

	return iLen;
}


int RecvData(int iFD, char *pDataBuf, unsigned int iBufSize, unsigned int iTimeOut)
{
	int iRet = COMM_RET_SUCCESS;
	unsigned int iLen = 0;
	
	while(iTimeOut > 0)
	{
		memset(pDataBuf, 0x00, iBufSize);

		iLen = recv(iFD, pDataBuf, DATE_BUF_LEN_SIZE, 0);
		if(iLen != DATE_BUF_LEN_SIZE)
		{
			iTimeOut--;
			sleep(1);
			iRet = COMM_RET_ERROR;
			continue;
		}

		iLen = WORD(pDataBuf[DATE_BUF_LEN_SIZE -2], pDataBuf[DATE_BUF_LEN_SIZE -1]);

		iLen= recv(iFD, pDataBuf + DATE_BUF_LEN_SIZE, iLen, 0);
		if(iLen < 0)
		{
			iTimeOut--;
			sleep(1);
			iRet = COMM_RET_TIMEOUT;
			continue;
		}
		else
		{
			iRet = iLen + DATE_BUF_LEN_SIZE;
			printf("TMS recv len = %d\n", iRet);
			break;
		}
	}

	return iRet;
}


