/***********************************************************************************
 * �� �� ��   : LandiCommTcp.h
 * �� �� ��   : harry
 * ��������   : 2016��3��2��
 * �ļ�����   : ���ں�TCP��ת��̬�����ͷ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#ifndef _LANDICOMMTCP_H_
#define _LANDICOMMTCP_H_


#define POS_COMMU_TYPE_COMM 1
#define POS_COMMU_TYPE_TCP 2

int StartCommTcpMap(int fdPOS, int iPosCommuType, int fdServer, char *szSoPath);//���ں�TCPӳ�����
int EndCommTcpMap(void);//��������TCP��ת

#endif

