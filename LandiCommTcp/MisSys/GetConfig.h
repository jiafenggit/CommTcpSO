/***********************************************************************************
 * �� �� ��   : GetConfig.h
 * �� �� ��   : harry
 * ��������   : 2016��3��11��
 * �ļ�����   : ��ȡ������Ϣ
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#ifndef _GET_CONFIG_H_
#define _GET_CONFIG_H_


#include "TypeDef.h"


//�����ļ���������
#define COMMU_PARA "COMMU_PARA"
#define PATH "PATH"

//�����ļ���������
#define TMS_SERVER_IP "TMS_SERVER_IP"
#define TMS_SERVER_PORT "TMS_SERVER_PORT"
#define MIS_COMMU_TYPE "MIS_COMMU_TYPE"
#define MIS_SERVER_PORT "MIS_SERVER_PORT"
#define SO_PATH "SO_PATH"

void GetStringContentValue(FILE *fp, UINT8 *pszSectionName, UINT8 *pszKeyName, UINT8 *pszOutput, UINT32 iOutputLen);
void GetConfigFileStringValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT8 *pDefaultVal, UINT8 *pszOutput, UINT32 iOutputLen);
INT32 GetConfigFileIntValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT32 iDefaultVal);


#endif


