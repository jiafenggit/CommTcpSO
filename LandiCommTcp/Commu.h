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
}CommPara;

#define DATE_BUF_LEN 36*1024

int initCommPara(int fdPOS, int iPosCommuType, int fdServer);//��ʼ��ͨѶ����
int SendToPos(char *pDataBuf, unsigned int iDataLen);//���ڷ�������
int RecvFormPos(char *pDataBuf, unsigned int iDataLen);//���ڽ�������
int SendToServer(char *pDataBuf, unsigned int iDataLen);//TCP��������
int RecvFormSever(char *pDataBuf, unsigned int iDataLen);//TCP��������
int CloseComm(void);//�ر�ͨѶ����

#endif

