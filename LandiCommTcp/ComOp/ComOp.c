#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>
#include<string.h>

#include "ComOp.h"




int UART_Open(int *fd, int port)
{
	int iUartFd = *fd;
	char *dev[] = {"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2"};

	iUartFd = open(dev[port - 1], O_RDWR|O_NOCTTY|O_NDELAY);
	if (-1 == iUartFd)
	{
		perror("Can't Open Serial Port");
		return -1;
	}

	if(fcntl(iUartFd, F_SETFL, 0) < 0)
	{
		printf("fcntl failed!\n");
		return -1;
	}

	if(0 == isatty(STDIN_FILENO))
	{
		printf("standard input is not a terminal device\n");
		return -1;
	}

	*fd = iUartFd;

	return 0;
}


void UART_Close(int fd)
{
	close(fd);
}


int UART_Set(int *fd, int speed, int flow_ctrl, int databits, int stopbits, int parity)
{
	int iUartFd = *fd;

	int i;
	int speed_arr[] =
	{
		B38400, B19200, B9600, B4800, B2400, B1200, B300,
		B38400, B19200, B9600, B4800, B2400, B1200, B300
	};
	int name_arr[] =
	{
		38400,  19200,  9600,  4800,  2400,  1200,  300, 38400,
		19200,  9600, 4800, 2400, 1200,  300
	};
	struct termios options;

	if(tcgetattr(iUartFd, &options)  !=  0)
	{
		perror("SetupSerial 1");
		return -1;
	}
	
	for(i= 0; i < sizeof(speed_arr) / sizeof(int); i++)
	{
		if  (speed == name_arr[i])
		{
			cfsetispeed(&options, speed_arr[i]);
			cfsetospeed(&options, speed_arr[i]);
		}
	}
	
	options.c_cflag |= CLOCAL;
	options.c_cflag |= CREAD;
	switch(flow_ctrl)
	{
	case 0 :
		options.c_cflag &= ~CRTSCTS;
		break;
	case 1 :
		options.c_cflag |= CRTSCTS;
		break;
	case 2 :
		options.c_cflag |= IXON | IXOFF | IXANY;
		break;
	}

	options.c_cflag &= ~CSIZE;
	switch (databits)
	{
	case 5 :
		options.c_cflag |= CS5;
		break;
	case 6	:
		options.c_cflag |= CS6;
		break;
	case 7	:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr,"Unsupported data size\n");
		return -1;
	}
	
	switch (parity)
	{
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case 's':
	case 'S':
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return -1;
	}
	
	switch (stopbits)
	{
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported stop bits\n");
		return -1;
	}

	options.c_oflag &= ~OPOST;

	options.c_cc[VTIME] = 1;
	options.c_cc[VMIN] = 1;

	tcflush(iUartFd,TCIFLUSH);

	if(tcsetattr(iUartFd,TCSANOW,&options) != 0)
	{
		perror("com set error!\n");
		return -1;
	}

	*fd = iUartFd;
	
	return 0;
}


int UART_Init(int *fd, int speed,int flow_ctrlint ,int databits,int stopbits,char parity)
{

	if (FALSE == UART_Set(fd,speed,flow_ctrlint,databits,stopbits,parity))
	{
		return FALSE;
	}
	else
	{
		return  TRUE;
	}
}

int UART_Recv(int fd, char *rcv_buf,int data_len, unsigned int iTimeOut)
{
	int len,fs_sel;
	fd_set fs_read;

	struct timeval time;

	FD_ZERO(&fs_read);
	FD_SET(fd,&fs_read);

	time.tv_sec = iTimeOut;
	time.tv_usec = 0;

	fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
	if(fs_sel)
	{
		len = read(fd,rcv_buf,data_len);
		return len;
	}
	else
	{
		return FALSE;
	}
}


int UART_Send(int fd, char *send_buf,int data_len)
{
	int ret;

	ret = write(fd,send_buf,data_len);
	if (data_len == ret )
	{
		return ret;
	}
	else
	{
		tcflush(fd,TCOFLUSH);
		return FALSE;
	}
}



