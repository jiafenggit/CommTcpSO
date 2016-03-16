/***********************************************************************************
 * �� �� ��   : TmsServer.c
 * �� �� ��   : harry
 * ��������   : 2016��3��15��
 * �ļ�����   : ģ��TMS������
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
 ����ִ�����:
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


#define WORD(h, l) ((h)*0x10 + l)


#define DATE_BUF_LEN_SIZE 2 //������ĳ���
#define SOCKET_DATA_SIZE 2048

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
	int iRet = COMM_RET_SUCCESS;
	int fdTms1 = 0, fdTms2 = 0;
	int len;
	struct sockaddr_in my_addr;   //�����������ַ�ṹ��
	struct sockaddr_in remote_addr; //�ͻ��������ַ�ṹ��
	int sin_size;
	char buf[SOCKET_DATA_SIZE];  //���ݴ��͵Ļ�����
	unsigned int iTime = 0;

	memset(&my_addr, 0, sizeof(my_addr)); //���ݳ�ʼ��--����
	my_addr.sin_family = AF_INET; //����ΪIPͨ��
	my_addr.sin_addr.s_addr = INADDR_ANY;//������IP��ַ--�������ӵ����б��ص�ַ��
	my_addr.sin_port = htons(8099); //TMS�������˿ں�

	/*�������������׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
	if((fdTms2 = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed\n");
		return -1;
	}

	/*���׽��ְ󶨵��������������ַ��*/
	if (bind(fdTms2, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind failed\n");
		return -2;
	}

	/*������������--�������г���Ϊ5*/
	listen(fdTms2, 5);

	sin_size = sizeof(struct sockaddr_in);

	/*�ȴ��ͻ����������󵽴�*/
	if((fdTms1 = accept(fdTms2, (struct sockaddr *)&remote_addr, &sin_size)) < 0)
	{
		perror("accept failed");
		return -3;
	}
	printf("accept client %s\n", inet_ntoa(remote_addr.sin_addr));

	//������������
	iRet = RecvData(fdTms1, buf, SOCKET_DATA_SIZE, RECV_TIME_OUT);
	if(iRet != COMM_RET_SUCCESS)
	{
		return -4;
	}

	if(len != strlen(START_STR))
	{
		close(fdTms1);
		close(fdTms2);
		return 0;
	}
	else
	{
		if(0 != strcmp(START_STR, buf + DATE_BUF_LEN_SIZE))
		{
			close(fdTms1);
			close(fdTms2);
			return 0;
		}
	}

	//��������
	len = SendData(fdTms1, "start tms download!", strlen("start tms download!"));

	//��������
	len = SendData(fdTms1, "TMS_FILE TMS_FILE ", strlen("TMS_FILE TMS_FILE "));

	//�������ؽ�������
	iRet = RecvData(fdTms1, buf, SOCKET_DATA_SIZE, RECV_TIME_OUT);
	if(iRet != COMM_RET_SUCCESS)
	{
		return -4;
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
	}

	close(fdTms1);
	close(fdTms2);

	return 0;
}


int SendData(int iFD, char *pDataBuf, unsigned int iDataLen)
{
	char SendBuf[SOCKET_DATA_SIZE] = {0};
	unsigned int iLen = 0;

	if(NULL == pDataBuf || 0 == iDataLen || iDataLen >= 0x100)
	{
		return -1;
	}

	SendBuf[DATE_BUF_LEN_SIZE -2] = iDataLen / 0x10;//��λ
	SendBuf[DATE_BUF_LEN_SIZE -1] = iDataLen % 0x10;//��λ
	iLen += DATE_BUF_LEN_SIZE;

	memcpy(SendBuf + DATE_BUF_LEN_SIZE, pDataBuf, iDataLen);
	iLen += iDataLen;

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
			printf("recv str:%s\n", pDataBuf + DATE_BUF_LEN_SIZE);
			iRet = iLen + DATE_BUF_LEN_SIZE;
			break;
		}
	}

	return iRet;
}


