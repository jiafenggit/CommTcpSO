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
#define COMMU_TYPE "COMMU_TYPE"
#define PATH "PATH"

//�����ļ���������
#define POS_COMMU_TYPE "PosCommuType"
#define SERVER_COMMU_TYPE "ServerCommuType"
#define SO_PATH "SoPath"

void GetStringContentValue(FILE *fp, UINT8 *pszSectionName, UINT8 *pszKeyName, UINT8 *pszOutput, UINT32 iOutputLen);
void GetConfigFileStringValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT8 *pDefaultVal, UINT8 *pszOutput, UINT32 iOutputLen);
INT32 GetConfigFileIntValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT32 iDefaultVal);


#endif


