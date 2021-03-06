/***********************************************************************************
 * 文 件 名   : LandiCommTcp.h
 * 负 责 人   : harry
 * 创建日期   : 2016年3月2日
 * 文件描述   : 串口和TCP互转动态库入口头文件
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/
#ifndef _LANDICOMMTCP_H_
#define _LANDICOMMTCP_H_


#define POS_COMMU_TYPE_COMM 1
#define POS_COMMU_TYPE_TCP 2

int StartCommTcpMap(int fdPOS, int iPosCommuType, int fdServer, unsigned int iTimeOut, char *szSoPath);//串口和TCP映射入口
int EndCommTcpMap(void);//结束串口TCP互转

#endif

