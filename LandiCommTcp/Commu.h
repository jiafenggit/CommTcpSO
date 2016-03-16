/***********************************************************************************
 * �� �� ��   : Commu.h
 * �� �� ��   : harry
 * ��������   : 2016��3��2��
 * �ļ�����   : ͨѶ��ز���ͷ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#ifndef _COMMU_H_
#define _COMMU_H_


//ͨѶ�����ṹ
typedef struct
{
	int fdPos;        //POSͨѶ���
	int iPosCommuType;//POSͨѶ����
	int fdServer;        //����ͨѶ���
	unsigned int iTimeOut; //ͨѶ�ȴ���ʱ
}CommPara;

//ͨѶ�������ؽ��
#define COMM_RET_SUCCESS	0
#define COMM_RET_ERROR		-1
#define COMM_RET_TIMEOUT	-2

#define DATE_BUF_LEN_SIZE 2 //������ĳ���
#define DATE_BUF_LEN 36*1024

#define START_STR "start"
#define END_STR "end"

int initCommPara(CommPara mCommPara);//��ʼ��ͨѶ����
int SendToPos(char *pDataBuf, unsigned int iDataLen);//���ڷ�������
int RecvFormPos(char *pDataBuf, unsigned int iDataLen);//���ڽ�������
int SendToServer(char *pDataBuf, unsigned int iDataLen);//TCP��������
int RecvFormSever(char *pDataBuf, unsigned int iDataLen);//TCP��������
int CloseComm(void);//�ر�ͨѶ����

#endif

