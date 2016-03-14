/*
±‡“Î÷¥––”Ôæ‰
gcc MisSys.c CommOp.c GetConfig.c -lpthread -o main ../libLandiCommTcp.so
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "../LandiCommTcp.h"

int main()
{
	int fdPos = 0, fdServer = 0;
	int iPosCommuType = 0;
	printf("running...\n");

	StartCommOp(&fdPos, &iPosCommuType, &fdServer);
	
	StartCommTcpMap(fdPos, iPosCommuType, fdServer, "../");

	return 0;
}

