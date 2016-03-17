/***********************************************************************************
 * �� �� ��   : MisPos.c
 * �� �� ��   : harry
 * ��������   : 2016��3��15��
 * �ļ�����   : ģ��MisPOS
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
 ����ִ�����:
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


#define DATE_BUF_LEN_SIZE 2 //������ĳ���
#define SOCKET_DATA_SIZE (4*1024)

#define RECV_TIME_OUT 30 //30��

//ͨѶ�������ؽ��
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
	struct sockaddr_in remote_addr; //�������������ַ�ṹ��
	char buf[SOCKET_DATA_SIZE];  //���ݴ��͵Ļ�����
	unsigned int iTime = 0;

	memset(&remote_addr, 0, sizeof(remote_addr)); //���ݳ�ʼ��--����
	remote_addr.sin_family = AF_INET; //����ΪIPͨ��
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//������IP��ַ
	remote_addr.sin_port = htons(8088); //MIS�������˿ں�

	/*�����ͻ����׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
	if((fdMis = socket(PF_INET, SOCK_STREAM, 0))<0)
	{
		perror("socket failed\n");
		return -1;
	}

	/*���׽��ְ󶨵��������������ַ��*/
	if(connect(fdMis, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr))<0)
	{
		perror("connect failed\n");
		return -2;
	}
	printf("connected to mis\n");

	printf("Press 'enter' to start TMS:\n");
	scanf("%s", buf);

	printf("start TMS!\n");

	//����TMS��������
	len = SendData(fdMis, START_STR, strlen(START_STR));

	//��������Ӧ��
	memset(buf,0x00, SOCKET_DATA_SIZE);
	iRet = RecvData(fdMis, buf, SOCKET_DATA_SIZE, RECV_TIME_OUT);
	if(iRet < COMM_RET_SUCCESS)
	{
		printf("2.RecvData failed!\n");
		return -7;
	}

	//��������
	memset(buf,0x00, SOCKET_DATA_SIZE);
	iRet = RecvData(fdMis, buf, SOCKET_DATA_SIZE, RECV_TIME_OUT);
	if(iRet < COMM_RET_SUCCESS)
	{
		printf("2.RecvData failed!\n");
		return -7;
	}

	//���ͽ�������
	len = SendData(fdMis, END_STR, strlen(END_STR));

	sleep(5);

	//�ر�ͨѶ���
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

	SendBuf[DATE_BUF_LEN_SIZE -2] = iDataLen / 0x100;//��λ
	SendBuf[DATE_BUF_LEN_SIZE -1] = iDataLen % 0x100;//��λ
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


