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
    char szSoLogPath[64] = {0};
    DIR *dirptr = NULL;
    unsigned char szTitle[32] = {0};
    unsigned char szTime[16] = {0};
    time_t t;
    struct tm *nowtime;

    //�ж��ļ����Ƿ����
    sprintf(szSoLogPath, "%s/LandiSoLog/", szSoPath);
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
    unsigned char wzLog[1024] = {0};
    unsigned char buffer[1024] = {0};
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


