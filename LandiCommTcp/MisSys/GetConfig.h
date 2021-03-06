/***********************************************************************************
 * 文 件 名   : GetConfig.h
 * 负 责 人   : harry
 * 创建日期   : 2016年3月11日
 * 文件描述   : 获取配置信息
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/
#ifndef _GET_CONFIG_H_
#define _GET_CONFIG_H_


#include "TypeDef.h"


//配置文件段名定义
#define COMMU_PARA "COMMU_PARA"
#define PATH "PATH"

//配置文件项名定义
#define TMS_SERVER_IP "TMS_SERVER_IP"
#define TMS_SERVER_PORT "TMS_SERVER_PORT"
#define MIS_COMMU_TYPE "MIS_COMMU_TYPE"
#define MIS_SERVER_PORT "MIS_SERVER_PORT"
#define SO_PATH "SO_PATH"

void GetStringContentValue(FILE *fp, UINT8 *pszSectionName, UINT8 *pszKeyName, UINT8 *pszOutput, UINT32 iOutputLen);
void GetConfigFileStringValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT8 *pDefaultVal, UINT8 *pszOutput, UINT32 iOutputLen);
INT32 GetConfigFileIntValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT32 iDefaultVal);


#endif


