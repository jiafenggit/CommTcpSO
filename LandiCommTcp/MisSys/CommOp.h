/***********************************************************************************
 * �� �� ��   : CommOp.h
 * �� �� ��   : harry
 * ��������   : 2016��3��14��
 * �ļ�����   : MISϵͳͨѶ����
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#ifndef _COMM_OP_H_
#define _COMM_OP_H_


//ͨѶ�����ṹ
typedef struct
{
	int fdPos;        //POSͨѶ���
	int iPosCommuType;//POSͨѶ����
	int fdServer;        //����ͨѶ���
}CommPara;

#define POS_COMMU_TYPE_COMM 1
#define  POS_COMMU_TYPE_TCP 2

int StartCommOp(int *fdPos, int *iPosCommuType, int *fdServer);//��ʼͨѶ���Ӳ���
int SocketClient(void);//����socket�ͻ���
int SocketServer(void);//����socket�����

#endif

