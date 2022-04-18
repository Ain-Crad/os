#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "error_functions.h"

#define BUF_SIZE 1024


char table[3][3][10] = {
    {"┌─","┬─","─┐"},
    {"├─","┼─","─┤"},
    {"└─","┴─","─┘"}
};

char hozn[] = "──";
char vert[] = "│ ";
char space[] = "  ";

void printTree();

typedef struct Process Process;

struct Process{
    char* name;
    pid_t pid;
    Process* first_child;
    Process* next_sib;
};

bool isNumber(const char* name){
    for(size_t i = 0; i < strlen(name); ++i){
        if(!isdigit(name[i])) return false;
    }
    return true;
}

char* getFilePath(const char* name){
    static char path[_POSIX_PATH_MAX + 1];
    
    sprintf(path, "/proc/%s/status", name);

    return path;
}

long getMaxPid(){
    static long max_pid = -1;
    if(max_pid == -1){
        FILE* f = fopen("/proc/sys/kernel/pid_max", "r");
        if(f == NULL){
            errExit("fopen");
        }
        if(fscanf(f, "%ld", &max_pid) != 1){
            errExit("fscanf");
        }
    }

    return max_pid;
}

Process* getProcess(int id){
    static Process* processes = NULL;
    
    if(processes == NULL){
        long size = getMaxPid() + 1;
        processes = malloc(sizeof(Process) * size);
        memset(processes, 0, sizeof(processes) * size);
    }

    return &processes[id];
}

int main(int argc, char* argv[]){
    bool PRINT_PID = false;

    if(argc > 1){
        for(int i = 1; i < argc; ++i){
            if(strcmp(argv[i], "-p") == 0){
                PRINT_PID = true;
            }else{
                printf("Usage error: pstree [-p]\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    DIR* proc_dir = opendir("/proc");
    if(proc_dir == NULL){
        errExit("opendir");
    }

    struct dirent* subdir;
    while((subdir = readdir(proc_dir)) != NULL){
        if(!isNumber(subdir->d_name)) continue;

        FILE* f = fopen(getFilePath(subdir->d_name), "r");
        if(f == NULL){
            errno = 0;
            continue;
        }

        // read content line by line
        char buf[BUF_SIZE];

        char proc_name[BUF_SIZE] = {'\0'};
        pid_t pid = 0;
        pid_t ppid = 0;
        
        int parsed = 0;

        while(fgets(buf, BUF_SIZE, f)){
            if(parsed >= 3) break;

            if(strncmp("Name:", buf, 5) == 0){
                int whitespace = 0;
                while(isspace(buf[5 + whitespace])) ++whitespace;
                strcpy(proc_name, buf + 5 + whitespace);
                int len = strlen(proc_name);
                proc_name[len - 1] = '\0';
                ++parsed;
            }else if(strncmp("Pid:", buf, 4) == 0){
                sscanf(buf + 4, "%d", &pid);
                ++parsed;
            }else if(strncmp("PPid:", buf, 5) == 0){
                sscanf(buf + 5, "%d", &ppid);
                ++parsed;
            }
        }

        assert(parsed == 3);
       
        
        if(PRINT_PID){
            char pid_str[10] = {'\0'};
            sprintf(pid_str, "(%d)", pid);
            strcat(proc_name, pid_str);
        }

        Process* parent = getProcess(ppid);
        Process* proc = getProcess(pid);
        proc->name = malloc(sizeof(char) * (strlen(proc_name) + 1));
        strcpy(proc->name, proc_name);
        proc->next_sib = parent->first_child;
        parent->first_child = proc;
    }

    printTree(getProcess(1), 0);

    return 0;
}

void printTree(Process* proc, int pos){
    static char* content[50000];

    content[pos++] = proc->name;

    int child_count = 0;
    for(Process* p = proc->first_child; p != NULL; p = p->next_sib){
        ++child_count;
    }

    if(child_count == 0){
        for(int i = 0; i < pos; ++i){
            printf("%s", content[i]);
        }
        printf("\n");

        for(int i = 1; i < pos; i += 2){
            if(content[i] == hozn) content[i] = space;
            else if(content[i] == table[0][1]) content[i] = vert;
            else if(content[i] == table[2][0]) content[i] = space;
            else if(content[i] == table[1][0]) content[i] = vert;
        }

        for(int i = 0; i < pos; i += 2){
            for(int j = 0; j < strlen(content[i]); ++j){
                content[i][j] = ' ';
            }
        }

        return;
    }

    int t = 0;
    for(Process* p = proc->first_child; p != NULL; p = p->next_sib, ++t){
        if(t == 0){
            content[pos++] = (p->next_sib == NULL) ? hozn : table[0][1];
        }else if(t == child_count - 1){
            content[pos++] = table[2][0];
        }else{
            content[pos++] = table[1][0];
        }

        printTree(p, pos);

        --pos;
    }
}
