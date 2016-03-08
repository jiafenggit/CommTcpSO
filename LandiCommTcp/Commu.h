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
	int fdComm;        //���ھ��
	int fdSocket;        //TCP���
}CommPara;


#define DATE_BUF_LEN 36*1024

int initCommPara(int fdComm, int fdSocket);//��ʼ��ͨѶ����
int CommSend(char *pDataBuf, unsigned int iDataLen);//���ڷ�������
int CommRecv(char *pDataBuf, unsigned int iDataLen);//���ڽ�������
int TcpSend(char *pDataBuf, unsigned int iDataLen);//TCP��������
int TcpRecv(char *pDataBuf, unsigned int iDataLen);//TCP��������
int CloseComm(void);//�ر�ͨѶ����

#endif

