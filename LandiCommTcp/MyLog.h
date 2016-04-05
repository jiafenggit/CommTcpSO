/***********************************************************************************
 * 文 件 名   : MyLog.h
 * 负 责 人   : harry
 * 创建日期   : 2016年3月8日
 * 文件描述   : 日志操作头文件
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#ifndef _MYLOG_H_
#define  _MYLOG_H_

//LOG_LEVEL
//#define LOG_LEVEL_DEBUG

void OpenMyLog(char *szSoPath);//打开日志文件
void CloseMyLog();//关闭日志
void LOG(const char* ms, ... );//记录日志
void mLogArrayPrint(unsigned int iLength, const void * pvData);//日志输出字符串数组内容

#endif

