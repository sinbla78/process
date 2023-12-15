#ifndef __LINUX_PROJECT_H__
#define __LINUX_PROJECT_H__

#include <sys/types.h>

#define MAX_STR 100
#define BLOCK_COUNT 10  //Max Sw Block number

#define LOGDIR  "./log"
#define LOGFILE "restart.txt"

typedef char* String;

typedef struct _swInfo {
    String name;    //process name
    String restart_count;   //restart count (char*)
    String start_time;  //start/restart time
    String reason;  //process signal/exit code
    int int_restart;    //restart count (int)
} swInfo;

typedef struct _swParam {
    String SwBlock;     // process name
    String App_para1;   // 1st parameter
    String App_para2;   // 2nd parameter
    String App_para3;   // 3rd parameter
} swParam;

typedef struct _swInfoManager {
    pid_t pids[BLOCK_COUNT];    //process pid array
    pid_t dpid; //dead pid
    int p_no;   //pid number
    swInfo sw_info[BLOCK_COUNT];    // struct swInfo array
    swParam sw_param[BLOCK_COUNT];  // struct swParam array
} swManager;

void InitStruct(swManager* info);
void readFileList(swManager* info);
void SpawnBlock(swManager* info);
void respawnProcess(swManager* info, int index);
void LogPrint(swManager* info);
int FindIndex(swManager* info);

char* rtrim(const char* s);
char* ltrim(const char* s);
char* trim(const char* s);
char *gettime(void);
void FileLogger(swInfo* list);
void DeadFinder();

#endif