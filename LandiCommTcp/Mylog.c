/***********************************************************************************
 * �� �� ��   : Mylog.c
 * �� �� ��   : harry
 * ��������   : 2016��3��8��
 * �ļ�����   : ��־����Դ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   :
 * �޸���־   :
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
 * �� �� ��     : OpenMyLog
 * �� �� ��     : harry
 * ��������  : 2016��3��8��
 * ��������  : ����־�ļ�
 * �������  : ��
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

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

	//��֯log�ļ�·��
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

	//�ж��ļ����Ƿ����
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
	//����־�ļ�
	printf("%s\n", szTitle);
	file = fopen(szTitle,"a+");
	printf("fopen succeed!\n");
	LOG("Start CommTcpMap so!");
}

/*****************************************************************************
 * �� �� ��     : CloseMyLog
 * �� �� ��     : harry
 * ��������  : 2016��3��8��
 * ��������  : �ر���־
 * �������  : ��
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
void CloseMyLog()
{
	fclose(file);
}

/*****************************************************************************
 * �� �� ��     : LOG
 * �� �� ��     : harry
 * ��������  : 2016��3��8��
 * ��������  : ��¼��־
 * �������  : unsigned char *szLogBuf  ��־����
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

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
 * �� �� ��     : mLogArrayPrint
 * �� �� ��     : harry
 * ��������  : 2016��3��22��
 * ��������  : ��־����ַ�����������
 * �������  : unsigned int iLength  ���ݳ���
               const void * pvData   ��������
 * �������  : ��
 * �� �� ֵ     :
 * ���ù�ϵ  :
 * ��    ��        :
 * ��    ��        :

*****************************************************************************/
void mLogArrayPrint(unsigned int iLength, const void * pvData)
{
#ifndef LOG_LEVEL_DEBUG
	return ;
#else
	//�����ж�
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

		//��ӡ����ֵ
		memset(szPrintBuf, 0x00, MAX_PRINT_SIZE);
		snprintf(szPrintBuf, MAX_PRINT_SIZE - 1, "DataLen = %d \r\n", iLength);
		iPrintLen = strlen(szPrintBuf) + 2;
		fwrite(szPrintBuf, 1, iPrintLen, file);

		for(i = 0; i < iLength; )
		{
			memset(szPrintBuf, 0x00, MAX_PRINT_SIZE);

			iOneLineLength = iLength - i > 16 ? 16 : iLength - i;//��ǰ��ʾ�еĳ���

			//��16���Ƹ�ʽ���������
			for(iCurIndex = 0; iCurIndex < iOneLineLength; iCurIndex++)
			{
				snprintf(&szPrintBuf[iCurIndex * 3], MAX_PRINT_SIZE - iCurIndex * 3, "%02X ", pcData[i + iCurIndex]);
			}

			//�󲹿ո�
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


