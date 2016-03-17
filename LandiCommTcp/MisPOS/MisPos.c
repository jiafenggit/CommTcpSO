/***********************************************************************************
 * 文 件 名   : MisPos.c
 * 负 责 人   : harry
 * 创建日期   : 2016年3月15日
 * 文件描述   : 模拟MisPOS
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   :
 * 修改日志   :
 编译执行语句:
 gcc MisPos.c -o MisPos
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
	int iRet = 0;
	int fdMis = 0;
	int len;
	struct sockaddr_in remote_addr; //服务器端网络地址结构体
	char buf[SOCKET_DATA_SIZE];  //数据传送的缓冲区
	unsigned int iTime = 0;

	memset(&remote_addr, 0, sizeof(remote_addr)); //数据初始化--清零
	remote_addr.sin_family = AF_INET; //设置为IP通信
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址
	remote_addr.sin_port = htons(8088); //MIS服务器端口号

	/*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
	if((fdMis = socket(PF_INET, SOCK_STREAM, 0))<0)
	{
		perror("socket failed\n");
		return -1;
	}

	/*将套接字绑定到服务器的网络地址上*/
	if(connect(fdMis, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr))<0)
	{
		perror("connect failed\n");
		return -2;
	}
	printf("connected to mis\n");

	printf("Press 'enter' to start TMS:\n");
	scanf("%s", buf);

	printf("start TMS!\n");

	//发起TMS下载请求
	len = SendData(fdMis, START_STR, strlen(START_STR));

	//接收握手应答
	memset(buf,0x00, SOCKET_DATA_SIZE);
	iRet = RecvData(fdMis, buf, SOCKET_DATA_SIZE, RECV_TIME_OUT);
	if(iRet < COMM_RET_SUCCESS)
	{
		printf("2.RecvData failed!\n");
		return -7;
	}

	//接收数据
	memset(buf,0x00, SOCKET_DATA_SIZE);
	iRet = RecvData(fdMis, buf, SOCKET_DATA_SIZE, RECV_TIME_OUT);
	if(iRet < COMM_RET_SUCCESS)
	{
		printf("2.RecvData failed!\n");
		return -7;
	}

	//发送结束报文
	len = SendData(fdMis, END_STR, strlen(END_STR));

	sleep(5);

	//关闭通讯句柄
	close(fdMis);

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

	printf("Pos send data len = %d\n", iLen);
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
			printf("Pos recv len = %d\n", iRet);
			break;
		}
	}

	return iRet;
}


