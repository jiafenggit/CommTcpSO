/*
±‡“Î÷¥––”Ôæ‰
gcc MisSys.c CommOp.c GetConfig.c -lpthread -o MisSys ../libLandiCommTcp.so
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "../LandiCommTcp.h"
#include "GetConfig.h"

#define RECV_TIME_OUT 30 //30√Î

int main()
{
	int iRet = 0;
	int fdPos = 0, fdServer = 0;
	int iPosCommuType = 0;
	char szSoPath[64] = {0};
	
	printf("running...\n");

	iRet = StartCommOp(&fdPos, &iPosCommuType, &fdServer);
	if(iRet < 0)
	{
		printf("StartCommOp failed!\n");
		return 0;
	}

	GetConfigFileStringValue(PATH, SO_PATH, "", szSoPath, sizeof(szSoPath));
	printf("szSoPath = %s\n", szSoPath);
	
	StartCommTcpMap(fdPos, iPosCommuType, fdServer, RECV_TIME_OUT, szSoPath);

	CloseCommu();

	return 0;
}

