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
    char szSoLogPath[64] = {0};
    DIR *dirptr = NULL;
    unsigned char szTitle[32] = {0};
    unsigned char szTime[16] = {0};
    time_t t;
    struct tm *nowtime;

    //判断文件夹是否存在
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


