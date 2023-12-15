#include "linuxProject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int status;
    char exitCode[50];

    swManager* mySwInfo = (swManager*)malloc(sizeof(swManager));
    InitStruct(mySwInfo); //변수 초기화

    readFileList(mySwInfo);
    SpawnBlock(mySwInfo);

    while(1) {
        mySwInfo->dpid = waitpid(-1, &status, 0);

        if(mySwInfo->dpid != -1) {
            int idx = FindIndex(mySwInfo);
            mySwInfo->sw_info[idx].int_restart++;
            sprintf(mySwInfo->sw_info[idx].restart_count, "%d", mySwInfo->sw_info[idx].int_restart);

            if(WIFEXITED(status)) {
                sprintf(exitCode, "EXIT(%d)", WEXITSTATUS(status));
                strcpy(mySwInfo->sw_info[idx].reason, exitCode);
            }
            else if(WIFSIGNALED(status)) {
                sprintf(exitCode, "SIGNAL(%d).%s", WTERMSIG(status), strsignal(WTERMSIG(status)));
                strcpy(mySwInfo->sw_info[idx].reason, exitCode);
            }

            respawnProcess(mySwInfo, idx);
            FileLogger(&(mySwInfo->sw_info[idx]));
            LogPrint(mySwInfo);
        }
    }
}