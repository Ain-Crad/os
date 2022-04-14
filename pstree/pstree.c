#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int pids[1000];

struct Node{
  int pid;
  struct Node* children[100];
};

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

struct Node* BuildTree(int cnt){ 
  for(int i = 0; i < cnt; ++i){
    char pid_str[10] = {'\0'};
    itoa(pids[i], pid_str);
    char file_name[20] = "/proc/";
    strcat(file_name, pid_str);
    printf("%s\n", file_name);
  }
  return NULL;
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

  struct Node* root = BuildTree(cnt);

  assert(root != NULL);
  return 0;
}
