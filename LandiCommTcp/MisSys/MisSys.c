/*
±‡“Î÷¥––”Ôæ‰
gcc main.c -lpthread -o main ../libLandiCommTcp.so 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "../LandiCommTcp.h"

int main()
{
    int fdComm = 0, fdTcp = 0;
    printf("running!\n");
    StartCommTcpMap(fdComm, fdTcp, "../");

    return 0;
}

