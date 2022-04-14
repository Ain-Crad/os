#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int pids[1000];

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

  fgets(header, len1, fp);
  int real_len = strlen(header);
  header[real_len - 1] = '\0';
  
  int pos = FindColon(header);
  
  // skip whitespace
  ++pos;
  while(isspace(header[pos])) ++pos;
  strcpy(content, header + pos);
}

void PrintTree(int (*graph)[100], char (*pid_name)[50], int pid, int pos, bool oneline){
  printf("%s %d\n", pid_name[pid], pos);

  int len = strlen(pid_name[pid]);

  //if(!oneline){
  //  for(int i = 0; i < pos; ++i) printf(" ");
  //}

  //if(pid != 1) for(int i = 0; i < 2; ++i) printf(" ");

  //printf("%s", pid_name[pid]);
  
  int cnt = 0;
  for(; graph[pid][cnt] != 0; ++cnt){
    int child = graph[pid][cnt];
    if(cnt == 0)
      PrintTree(graph, pid_name, child, pos + len, true);
    else
      PrintTree(graph, pid_name, child, pos + len, false);
  }

  //if(cnt == 0){
  //  printf("\n");
  //}
}

void BuildTree(int cnt){
  int max_pid = 0;
  for(int i = 0; i < cnt; ++i){
    max_pid = pids[i] > max_pid ? pids[i] : max_pid;
  }

  int par[max_pid + 1];
  memset(par, 0, sizeof(par));

  char pid_name[max_pid + 1][50];
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

  int end_pos[max_pid + 1];
  memset(end_pos, 0, sizeof(end_pos));

  int graph[max_pid + 1][100];
  memset(graph, 0, sizeof(graph));

  for(int i = 0; i < cnt; ++i){
    int pid = pids[i];
    int ppid = par[pid];
    if(ppid == 0) continue;
    int end = end_pos[ppid];
    graph[ppid][end] = pid;
    ++end_pos[ppid];
  }
  
  PrintTree(graph, pid_name, 1, 0, true);
}

int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);

  DIR* dp = opendir("/proc");
  if(dp == NULL){
    printf("Couldn't open directory\n");
    exit(EXIT_FAILURE);
  }

  struct dirent* ep;
  int cnt = 0;
  while((ep = readdir(dp)) != NULL){
    if(IsPid(ep->d_name)){
      pids[cnt++] = atoi(ep->d_name);
    }
  }

  closedir(dp);
 
  BuildTree(cnt);

  return 0;
}
