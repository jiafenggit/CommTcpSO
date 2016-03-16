/***********************************************************************************
 * 文 件 名   : Commu.h
 * 负 责 人   : harry
 * 创建日期   : 2016年3月2日
 * 文件描述   : 通讯相关操作头文件
 * 版权说明   : Copyright (c) 2008-2016   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/
#ifndef _COMMU_H_
#define _COMMU_H_


//通讯参数结构
typedef struct
{
	int fdPos;        //POS通讯句柄
	int iPosCommuType;//POS通讯类型
	int fdServer;        //服务通讯句柄
	unsigned int iTimeOut; //通讯等待超时
}CommPara;

//通讯操作返回结果
#define COMM_RET_SUCCESS	0
#define COMM_RET_ERROR		-1
#define COMM_RET_TIMEOUT	-2

#define DATE_BUF_LEN_SIZE 2 //长度域的长度
#define DATE_BUF_LEN 36*1024

#define START_STR "start"
#define END_STR "end"

int initCommPara(CommPara mCommPara);//初始化通讯参数
int SendToPos(char *pDataBuf, unsigned int iDataLen);//串口发送数据
int RecvFormPos(char *pDataBuf, unsigned int iDataLen);//串口接收数据
int SendToServer(char *pDataBuf, unsigned int iDataLen);//TCP发送数据
int RecvFormSever(char *pDataBuf, unsigned int iDataLen);//TCP接收数据
int CloseComm(void);//关闭通讯连接

#endif

