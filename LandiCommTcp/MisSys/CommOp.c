/***********************************************************************************
 * �� �� ��   : CommOp.c
 * �� �� ��   : harry
 * ��������   : 2016��3��14��
 * �ļ�����   : MISϵͳͨѶ����
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
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

//ͨѶ����ȫ�ֱ���
static CommPara s_CommPara;
static int s_fdPos1 = 0;
static int s_fdPos2 = 0;
static int s_fdServer = 0;

#define SOCKET_DATA_SIZE (28*1024)

/*****************************************************************************
 * �� �� ��     : StartCommOp
 * �� �� ��     : harry
 * ��������  : 2016��3��14��
 * ��������  : ��ʼͨѶ���Ӳ���
 * �������  : int *fdPos          POS��ͨѶ���ָ��
               int *iPosCommuType  POS��ͨѶ����ָ��
               int *fdServer       �����ͨѶ���ָ��
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

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
 * �� �� ��     : SocketClient
 * �� �� ��     : harry
 * ��������  : 2016��3��14��
 * ��������  : ����socket�ͻ���
 * �������  : ��
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int SocketClient(void)
{
	int len, iRet;
	struct sockaddr_in remote_addr; //�������������ַ�ṹ��
	char buf[SOCKET_DATA_SIZE];  //���ݴ��͵Ļ�����
	char TmsServerIp[16] = {0};
	unsigned int TmsServerPort = 8000;

	GetConfigFileStringValue(COMMU_PARA, TMS_SERVER_IP, "127.0.0.1", TmsServerIp, sizeof(TmsServerIp));
	TmsServerPort = GetConfigFileIntValue(COMMU_PARA, TMS_SERVER_PORT, 8000);
	printf("TmsServerIP = %s, port = %d\n", TmsServerIp, TmsServerPort);

	memset(&remote_addr, 0, sizeof(remote_addr)); //���ݳ�ʼ��--����
	remote_addr.sin_family = AF_INET; //����ΪIPͨ��
	remote_addr.sin_port = htons(TmsServerPort); //TMS�������˿ں�
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

	/*�����ͻ����׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
	if((s_fdServer = socket(PF_INET, SOCK_STREAM, 0))<0)
	{
		perror("socket failed");
		return -1;
	}

	/*���׽��ְ󶨵��������������ַ��*/
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
 * �� �� ��     : SocketServer
 * �� �� ��     : harry
 * ��������  : 2016��3��14��
 * ��������  : ����socket�����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int SocketServer(void)
{
	int len;
	struct sockaddr_in my_addr;   //�����������ַ�ṹ��
	struct sockaddr_in remote_addr; //�ͻ��������ַ�ṹ��
	int sin_size;
	char buf[SOCKET_DATA_SIZE];  //���ݴ��͵Ļ�����
	unsigned int MisServerPort = 8000;

	//��ȡ�����ļ�
	MisServerPort = GetConfigFileIntValue(COMMU_PARA, MIS_SERVER_PORT, 8000);
	printf("MisServerPort = %d \n", MisServerPort);

	memset(&my_addr, 0, sizeof(my_addr)); //���ݳ�ʼ��--����
	my_addr.sin_family = AF_INET; //����ΪIPͨ��
	my_addr.sin_addr.s_addr = INADDR_ANY;//������IP��ַ--�������ӵ����б��ص�ַ��
	my_addr.sin_port = htons(MisServerPort); //�������˿ں�

	/*�������������׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
	if((s_fdPos2 = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		return -1;
	}

	int flags = fcntl(s_fdPos2, F_GETFL, 0);
	fcntl(s_fdPos2, F_SETFL, flags | ~O_NONBLOCK);

	/*���׽��ְ󶨵��������������ַ��*/
	if (bind(s_fdPos2, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind failed");
		return -2;
	}

	/*������������--�������г���Ϊ5*/
	listen(s_fdPos2, 5);

	sin_size = sizeof(struct sockaddr_in);

	/*�ȴ��ͻ����������󵽴�*/
	if((s_fdPos1 = accept(s_fdPos2, (struct sockaddr *)&remote_addr, &sin_size)) < 0)
	{
		perror("accept failed");
		return -3;
	}
	printf("accept client %s\n", inet_ntoa(remote_addr.sin_addr));

//	len = send(client_sockfd, "Welcome to my server/n",21,0);//���ͻ�ӭ��Ϣ
//	len = recv(client_sockfd, buf, BUFSIZ, 0);

	return 0;
}


/*****************************************************************************
 * �� �� ��     : ComClient
 * �� �� ��     : harry
 * ��������  : 2016��8��16��
 * ��������  : �������ڿͻ���
 * �������  : ��
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int ComClient(void)
{
	unsigned int TmsServerPort = 8000;
	
	TmsServerPort = GetConfigFileIntValue(COMMU_PARA, TMS_SERVER_PORT, 8000);
	printf("TmsServerPort = %d\n", TmsServerPort);
}

/*****************************************************************************
 * �� �� ��     : ComServer
 * �� �� ��     : harry
 * ��������  : 2016��8��16��
 * ��������  : �������ڷ����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
int ComServer(void)
{
	unsigned int MisServerPort = 8000;
	
	MisServerPort = GetConfigFileIntValue(COMMU_PARA, MIS_SERVER_PORT, 8000);
	printf("MisServerPort = %d \n", MisServerPort);
}

/*****************************************************************************
 * �� �� ��     : CloseCommu
 * �� �� ��     : harry
 * ��������  : 2016��3��15��
 * ��������  : �ر�socket���
 * �������  : ��
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
void CloseCommu(void)
{
	close(s_fdPos1);
	close(s_fdPos2);
	close(s_fdServer);
}

