#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linuxProject.h"

void InitStruct(swManager* info) { //Init struct swManager
    info->p_no = 0;
    info->dpid = 0;
    for(int i = 0; i < BLOCK_COUNT; i++) {
        info->pids[i] = 0;
        info->sw_info[i].name = (String)malloc(sizeof(char)*20);
        info->sw_info[i].reason = (String)malloc(sizeof(char)*50);
        info->sw_info[i].restart_count = (String)malloc(sizeof(char)*20);
        info->sw_info[i].int_restart = 0;
        info->sw_info[i].start_time = (String)malloc(sizeof(char)*30);
        info->sw_param[i].SwBlock = (String)malloc(sizeof(char)*20);
        info->sw_param[i].App_para1 = (String)malloc(sizeof(char)*20);
        info->sw_param[i].App_para2 = (String)malloc(sizeof(char)*20);
        info->sw_param[i].App_para3 = (String)malloc(sizeof(char)*20);
    }
}

void readFileList(swManager* info) {
    FILE* fp;
    fp = fopen("./blockList.txt", "r"); //블록리스트 열기(RO)
    char str[MAX_STR];
    int swno = 0;

    while(fgets(str, MAX_STR, fp)) {
        String result;
        str[strlen(str)-1] = 0; // cut \n
        
        result = strtok(str, ";");
        strcpy(result, trim(result));

        strcpy(info->sw_param[swno].SwBlock, result);

        for(int i = 0; result == strtok(NULL, ";"); i++) {
            strcpy(result, trim(result));

            switch (i) {
            case 0:
                strcpy(info->sw_param[swno].App_para1, result);
                break;
            case 1:
                strcpy(info->sw_param[swno].App_para2, result);
                break;
            case 2:
                strcpy(info->sw_param[swno].App_para3, result);
                break;
            
            default:
                break;
            }
        }

        swno++;
    }

    info->p_no = swno;
}

void SpawnBlock(swManager* info) {
    pid_t pid;
    int i, status;

    for(i = 0; i < info->p_no; i++) {
        strcpy(info->sw_info[i].name, info->sw_param[i].SwBlock);
        sprintf(info->sw_info[i].reason, "Init.");
        sprintf(info->sw_info[i].restart_count, "%d", 0);
        strcpy(info->sw_info[i].start_time, gettime());

        FileLogger(&(info->sw_info[i]));
        LogPrint(info);

        pid = fork();

        if(pid > 0) { //p
            info->pids[i] = pid; 
        }
        else if(pid == 0) {
            char path[30] = "./";
            strcat(path, info->sw_param[i].SwBlock);
            execl(path, info->sw_param[i].SwBlock, 
            info->sw_param[i].App_para1, 
            info->sw_param[i].App_para2, 
            info->sw_param[i].App_para3, NULL);
        }
    }
}

void respawnProcess(swManager* info, int index) {
    pid_t pid;

    pid = fork();

    if(pid > 0) {
        info->pids[index] = pid;
        strcpy(info->sw_info[index].start_time, gettime());
    }
    else if(pid == 0) {
        char path[30] = "./";
        strcat(path, info->sw_param[index].SwBlock);
        execl(path, info->sw_param[index].SwBlock,
        info->sw_param[index].App_para1, 
        info->sw_param[index].App_para2, 
        info->sw_param[index].App_para3, NULL);
    }
}

int FindIndex(swManager* info) {
    for(int i = 0; i < info->p_no; i++) {
        if(info->dpid == info->pids[i]) {
            return i;
        }
    }

    return -1;
}

void FileLogger(swInfo* list) {
    mkdir(LOGDIR, 0755);
    chdir(LOGDIR);

    FILE* fp;

    fp = fopen(LOGFILE, "a");

    fprintf(fp, "process name: %s\trestart count: %s\tstart time: %s\treason: %s\n",
                    list->name, list->restart_count, list->start_time, list->reason);

    fclose(fp);  

    chdir("../");
}

void LogPrint(swManager* info) {
    char param[10];

    system("clear");
    printf(" _____________________________________________________________________________________________ \n");
    printf("|  Block name  | Restart count |       Start time        |               Reason               |\n");
    printf("|______________|_______________|_________________________|____________________________________|\n");

    for(int i = 0; i < info->p_no; i++) {
        printf("| %12s |", info->sw_info[i].name);
        printf(" %13s |", info->sw_info[i].restart_count);
        printf(" %s|", info->sw_info[i].start_time);
        printf(" %34s |\n", info->sw_info[i].reason);
    }

    printf("|______________|_______________|_________________________|____________________________________|\n");
}

char* rtrim(const char* s) {
    while(isspace(*s) || !isprint(*s)) ++s;
    return strdup(s);
}

char* ltrim(const char* s) {
    char* r = strdup(s);
    if(r != NULL) {
        char* fr = r + strlen(s) - 1;
        while((isspace(*fr) || !isprint(*fr) || *fr == 0) && fr >= r) --fr;
        *++fr = 0;
    }
    return r;
}

char* trim(const char* s) {
    char* r = rtrim(s);
    char* f = rtrim(r);
    free(r);
    return f;
}

char *gettime(void) {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    time_t t = (time_t)tv.tv_sec;

    struct tm *ptm = localtime(&t);

    static char str[1024];

    sprintf(str, "%04d.%02d.%02d %02d:%02d:%02d.%03d ",

            ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,

            ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (int)tv.tv_usec / 1000);

    return str;
}
