#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int pids[1000];
int cnt;
char pid_name[40000][50];

int graph[40000][100];
int end_pos[40000];
int par[40000];

char res[1000][1000];
int row;

bool PRINT_PID;

bool IsPid(char* name){
    char* ch = name;
    for(; *ch; ++ch){
        if(!isdigit(*ch)){
            return false;
        }
    }
    return true;
}

void reverse_str(char* str){
    int len = strlen(str);
    int l = 0, r = len - 1;
    while(l < r){
        char tmp = str[l];
        str[l] = str[r];
        str[r] = tmp;
        
        ++l;
        --r;
    }
}

char* itoa(int val, char* str){
    int p = 0;
    while(val){
        int x = val % 10;
        val /= 10;
        str[p++] = '0' + x;
    }
    reverse_str(str);
    return str;
}

int FindColon(char* line){
    int colon_pos = 0;
    while(line[colon_pos] != ':') ++colon_pos;
    line[colon_pos] = '\0';
    return colon_pos;
}

void GetContent(FILE* fp, char* header, int len1, char* content, int len2){
    memset(header, 0, len1 * sizeof(char));
    memset(content, 0, len2 * sizeof(char));

    fgets(header, len1, fp); int real_len = strlen(header);
    header[real_len - 1] = '\0';
    
    int pos = FindColon(header);
    
    // skip whitespace
    ++pos;
    while(isspace(header[pos])) ++pos;
    strcpy(content, header + pos);
}

void PrintTree(int pid, int pos, bool oneline){
    int len = strlen(pid_name[pid]);
    
    char pid_str[10];
    memset(pid_str, 0, sizeof(pid_str));
    itoa(pid, pid_str);
    if(PRINT_PID) len += strlen(pid_str) + 2;

    if(!oneline){
        for(int i = 0; i < pos; ++i) printf(" ");
    }

    if(pid != 1){
        for(int i = 0; i < 3; ++i) printf(" ");
    }

    printf("%s", pid_name[pid]);
    if(PRINT_PID) printf("(%s)", pid_str);
    
    int child_idx = 0;
    for(; graph[pid][child_idx] != 0; ++child_idx){
        int child = graph[pid][child_idx];
        
        int pre = (pid == 1 ? 0 : 3); 
        if(child_idx == 0){
            PrintTree(child, pos + pre + len, true);
        }else{
            PrintTree(child, pos + pre + len, false);
        }
    }

    if(child_idx == 0){
        printf("\n");
    }
}

void FillRes(int pid, int pos, bool oneline){
    int cur_pos = pos;
    
    char pid_str[10];
    memset(pid_str, 0, sizeof(pid_str));
    itoa(pid, pid_str);
    
    if(!oneline){
        for(int i = 0; i < pos; ++i) res[row][i] = ' ';
    }

    if(pid != 1){
        if(oneline) sprintf(&(res[row][cur_pos]), "---");
        else sprintf(&(res[row][cur_pos]), " |-");
        cur_pos += 3;
    }
    
    sprintf(&(res[row][cur_pos]), "%s", pid_name[pid]);
    cur_pos += strlen(pid_name[pid]);

    if(PRINT_PID){
        sprintf(&(res[row][cur_pos]), "(%s)", pid_str);
        cur_pos += strlen(pid_str) + 2;
    }

    int child_idx = 0;
    for(; graph[pid][child_idx] != 0; ++child_idx){
        int child = graph[pid][child_idx];

        int pre = (pid == 1 ? 0 : 3);
        if(child_idx == 0){
            FillRes(child, cur_pos, true);
        }else{
            FillRes(child, cur_pos, false);
        }
    }
    
    if(child_idx == 0){
        ++row;
    }
}

void PrintTree2(int pid, int pos, bool oneline){
    FillRes(pid, pos, oneline);
    int i = 0;
    while(res[i] && strlen(res[i]) > 0){
        printf("%s\n", res[i]);
        ++i;
    }
}

void BuildTree(){
    memset(end_pos, 0, sizeof(end_pos));
    memset(graph, 0, sizeof(graph));

    for(int i = 0; i < cnt; ++i){
        int pid = pids[i];
        int ppid = par[pid];
        if(ppid == 0) continue;
        int end = end_pos[ppid];
        graph[ppid][end] = pid;
        ++end_pos[ppid];
    }
}

void ReadProcStatus(){
    memset(par, 0, sizeof(par));
    memset(pid_name, 0, sizeof(pid_name));
    
    for(int i = 0; i < cnt; ++i){
        char pid_str[10] = {'\0'};
        itoa(pids[i], pid_str);
        char file_name[30] = "/proc/";
        strcat(file_name, pid_str);
        strcat(file_name, "/status");

        // printf("%s\n", file_name);
        
        FILE* fp = fopen(file_name, "r");
        if(fp == NULL){
            printf("Couldn't open file %s", file_name);
            exit(EXIT_FAILURE);
        }
        
        char header[50] = {'\0'};
        char content[50] = {'\0'};
        // get name
        char name[50] = {'\0'};
        GetContent(fp, header, 50, content, 50);
        assert(strcmp(header, "Name") == 0);
        strcpy(name, content);
        // skip State and Tgid
        GetContent(fp, header, 50, content, 50);
        assert(strcmp(header, "State") == 0);
        GetContent(fp, header, 50, content, 50);
        assert(strcmp(header, "Tgid") == 0);
        // get pid
        GetContent(fp, header, 50, content, 50);
        assert(strcmp(header, "Pid") == 0);
        int pid = atoi(content);
        // get ppid
        GetContent(fp, header, 50, content, 50);
        assert(strcmp(header, "PPid") == 0);
        int ppid = atoi(content);

        // printf("%s %d %d\n", name, pid, ppid);
        
        par[pid] = ppid;

        strcpy(pid_name[pid], name);

        fclose(fp);
    }    
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        assert(argv[i]);
        //printf("argv[%d] = %s\n", i, argv[i]);
    }
    assert(!argv[argc]);

    PRINT_PID = false;
 
    for(int i = 1; i < argc; ++i){
        assert(argv[i]);
        if(strcmp(argv[i], "-p") == 0){
            PRINT_PID = true;        
        }else{
            printf("usage error! usage: pstree-* [-p]\n");
            exit(EXIT_FAILURE);
        }
    }

    DIR* dp = opendir("/proc");
    if(dp == NULL){
        printf("Couldn't open directory\n");
        exit(EXIT_FAILURE);
    }

    struct dirent* ep;
    cnt = 0;
    while((ep = readdir(dp)) != NULL){
        if(IsPid(ep->d_name)){
            pids[cnt++] = atoi(ep->d_name);
        }
    }

    closedir(dp);
 
    ReadProcStatus();
    BuildTree();
    //PrintTree(1, 0, true);

    memset(res, 0, sizeof(res));

    row = 0;
    PrintTree2(1, 0, true);

    return 0;
}
