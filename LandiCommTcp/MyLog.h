/***********************************************************************************
 * �� �� ��   : MyLog.h
 * �� �� ��   : harry
 * ��������   : 2016��3��8��
 * �ļ�����   : ��־����ͷ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#ifndef _MYLOG_H_
#define  _MYLOG_H_

//LOG_LEVEL
//#define LOG_LEVEL_DEBUG

void OpenMyLog(char *szSoPath);//����־�ļ�
void CloseMyLog();//�ر���־
void LOG(const char* ms, ... );//��¼��־
void mLogArrayPrint(unsigned int iLength, const void * pvData);//��־����ַ�����������

#endif

