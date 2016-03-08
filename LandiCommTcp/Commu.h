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
	int fdComm;        //串口句柄
	int fdSocket;        //TCP句柄
}CommPara;


#define DATE_BUF_LEN 36*1024

int initCommPara(int fdComm, int fdSocket);//初始化通讯参数
int CommSend(char *pDataBuf, unsigned int iDataLen);//串口发送数据
int CommRecv(char *pDataBuf, unsigned int iDataLen);//串口接收数据
int TcpSend(char *pDataBuf, unsigned int iDataLen);//TCP发送数据
int TcpRecv(char *pDataBuf, unsigned int iDataLen);//TCP接收数据
int CloseComm(void);//关闭通讯连接

#endif

