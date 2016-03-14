/***********************************************************************************
 * 文 件 名   : CommOp.h
 * 负 责 人   : harry
 * 创建日期   : 2016年3月14日
 * 文件描述   : MIS系统通讯操作
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/
#ifndef _COMM_OP_H_
#define _COMM_OP_H_


//通讯参数结构
typedef struct
{
	int fdPos;        //POS通讯句柄
	int iPosCommuType;//POS通讯类型
	int fdServer;        //服务通讯句柄
}CommPara;

#define POS_COMMU_TYPE_COMM 1
#define  POS_COMMU_TYPE_TCP 2

int StartCommOp(int *fdPos, int *iPosCommuType, int *fdServer);//开始通讯连接操作
int SocketClient(void);//创建socket客户端
int SocketServer(void);//创建socket服务端

#endif

