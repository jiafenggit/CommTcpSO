/***********************************************************************************
 * 文 件 名   : Mylog.c
 * 负 责 人   : harry
 * 创建日期   : 2016年3月8日
 * 文件描述   : 日志功能源文件
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   :
 * 修改日志   :
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <stdarg.h>
#include <dirent.h>

#include "MyLog.h"

static FILE* file = NULL;


#define LOG_BUF_LEN 1024
#define MAX_PRINT_SIZE 128
#define PRINT_ADDR_LEN 11

/*****************************************************************************
 * 函 数 名     : OpenMyLog
 * 负 责 人     : harry
 * 创建日期  : 2016年3月8日
 * 函数功能  : 打开日志文件
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
void OpenMyLog(char *szSoPath)
{
	unsigned int iSoPathLen = 0;
	char szSoLogPath[64] = {0};
	DIR *dirptr = NULL;
	unsigned char szTitle[32] = {0};
	unsigned char szTime[16] = {0};
	time_t t;
	struct tm *nowtime;

	if(NULL == szSoPath)
	{
		printf("SoPath is NULL!\n");
		return ;
	}
	else
	{
		strcpy(szSoLogPath, szSoPath);
	}

	//组织log文件路径
	iSoPathLen = strlen(szSoLogPath);
	printf("szSoLogPath = %s, iSoLogPathLen = %d\n", szSoLogPath, iSoPathLen);
	if(szSoLogPath[iSoPathLen - 1] == '/')
	{
		sprintf(szSoLogPath, "%sLandiSoLog/", szSoLogPath);
	}
	else
	{
		sprintf(szSoLogPath, "%s/LandiSoLog/", szSoLogPath);
	}
	printf("%s\n", szSoLogPath);

	//判断文件夹是否存在
	if((dirptr = opendir(szSoLogPath)) == NULL)
	{
		if(mkdir(szSoLogPath, 0777) < 0)
		{
			printf("mkdir failed! \n");
			return ;
		}
	}

	time(&t);
	nowtime = localtime(&t);
	strftime(szTime,sizeof(szTime),"%Y-%m-%d",nowtime);
	sprintf(szTitle, "%sLandiCommTcpLog%s.log", szSoLogPath, szTime);
	//打开日志文件
	printf("%s\n", szTitle);
	file = fopen(szTitle,"a+");
	printf("fopen succeed!\n");
	LOG("Start CommTcpMap so!");
}

/*****************************************************************************
 * 函 数 名     : CloseMyLog
 * 负 责 人     : harry
 * 创建日期  : 2016年3月8日
 * 函数功能  : 关闭日志
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
void CloseMyLog()
{
	fclose(file);
}

/*****************************************************************************
 * 函 数 名     : LOG
 * 负 责 人     : harry
 * 创建日期  : 2016年3月8日
 * 函数功能  : 记录日志
 * 输入参数  : unsigned char *szLogBuf  日志内容
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
void LOG(const char* ms, ... )
{
	unsigned char wzLog[LOG_BUF_LEN] = {0};
	unsigned char buffer[LOG_BUF_LEN] = {0};
	va_list args;
	va_start(args, ms);
	vsprintf( wzLog ,ms,args);
	va_end(args);

	time_t now;
	time(&now);
	struct tm *local;
	local = localtime(&now);
	printf("%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon,
	       local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,
	       wzLog);
	sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon,
	        local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,
	        wzLog);

	fwrite(buffer,1, strlen(buffer), file);

	return ;
}

/*****************************************************************************
 * 函 数 名     : mLogArrayPrint
 * 负 责 人     : harry
 * 创建日期  : 2016年3月22日
 * 函数功能  : 日志输出字符串数组内容
 * 输入参数  : unsigned int iLength  数据长度
               const void * pvData   数据内容
 * 输出参数  : 无
 * 返 回 值     :
 * 调用关系  :
 * 其    它        :
 * 修    改        :

*****************************************************************************/
void mLogArrayPrint(unsigned int iLength, const void * pvData)
{
#ifndef LOG_LEVEL_DEBUG
	return ;
#else
	//参数判断
	if(NULL == pvData || 0 == iLength)
	{
		return;
	}

	do
	{
		const unsigned char *pcData = (const unsigned char *) pvData;
		char szPrintBuf[MAX_PRINT_SIZE] = {0};
		unsigned int iPrintLen = 0;
		unsigned int i = 0;
		unsigned int iOneLineLength = 0;
		unsigned int iCurIndex = 0;

		//打印长度值
		memset(szPrintBuf, 0x00, MAX_PRINT_SIZE);
		snprintf(szPrintBuf, MAX_PRINT_SIZE - 1, "DataLen = %d \r\n", iLength);
		iPrintLen = strlen(szPrintBuf) + 2;
		fwrite(szPrintBuf, 1, iPrintLen, file);

		for(i = 0; i < iLength; )
		{
			memset(szPrintBuf, 0x00, MAX_PRINT_SIZE);

			iOneLineLength = iLength - i > 16 ? 16 : iLength - i;//当前显示行的长度

			//以16进制格式化输出数据
			for(iCurIndex = 0; iCurIndex < iOneLineLength; iCurIndex++)
			{
				snprintf(&szPrintBuf[iCurIndex * 3], MAX_PRINT_SIZE - iCurIndex * 3, "%02X ", pcData[i + iCurIndex]);
			}

			//后补空格
			for(; iCurIndex < 16; iCurIndex++)
			{
				memset(&szPrintBuf[iCurIndex * 3], ' ', 3);
			}

			szPrintBuf[16 * 3] = ' ';
			szPrintBuf[16 * 3 + 1] = ' ';

			strcpy(&szPrintBuf[16 * 3 + 2], "\r\n");

			iPrintLen = 16 * 3 + 2 + 2;
			i += iOneLineLength;
			fwrite(szPrintBuf, 1, iPrintLen, file);
		}

	}
	whilE(0);
#endif
}


