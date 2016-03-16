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

//ͨѶ����ȫ�ֱ���
static CommPara s_CommPara;
static int s_fdPos1 = 0;
static int s_fdPos2 = 0;
static int s_fdServer = 0;

#define SOCKET_DATA_SIZE 2048

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

	iMisCommType = GetConfigFileIntValue(COMMU_TYPE, POS_COMMU_TYPE, 0);

	if(POS_COMMU_TYPE_TCP == iMisCommType)
	{
		iRet = SocketServer();
		if(iRet < 0)
		{
			return iRet;
		}
	}

	iRet = SocketClient();
	if(iRet < 0)
	{
		return iRet;
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
	int len;
	struct sockaddr_in remote_addr; //�������������ַ�ṹ��
	char buf[SOCKET_DATA_SIZE];  //���ݴ��͵Ļ�����

	memset(&remote_addr, 0, sizeof(remote_addr)); //���ݳ�ʼ��--����
	remote_addr.sin_family = AF_INET; //����ΪIPͨ��
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//������IP��ַ
	remote_addr.sin_port = htons(8099); //TMS�������˿ں�

	/*�����ͻ����׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
	if((s_fdServer = socket(PF_INET, SOCK_STREAM, 0))<0)
	{
		perror("socket failed");
		return -1;
	}

	/*���׽��ְ󶨵��������������ַ��*/
	if(connect(s_fdServer, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr))<0)
	{
		perror("connect failed");
		return -2;
	}
	printf("connected to server/n");

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

	memset(&my_addr, 0, sizeof(my_addr)); //���ݳ�ʼ��--����
	my_addr.sin_family = AF_INET; //����ΪIPͨ��
	my_addr.sin_addr.s_addr = INADDR_ANY;//������IP��ַ--�������ӵ����б��ص�ַ��
	my_addr.sin_port = htons(8088); //�������˿ں�

	/*�������������׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
	if((s_fdPos2 = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		return -1;
	}

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
	printf("accept client %s/n", inet_ntoa(remote_addr.sin_addr));

//	len = send(client_sockfd, "Welcome to my server/n",21,0);//���ͻ�ӭ��Ϣ
//	len = recv(client_sockfd, buf, BUFSIZ, 0);

	return 0;
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

