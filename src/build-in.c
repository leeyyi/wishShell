#include"build-in.h"
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include"parser.h"
#include<stdlib.h>
void add_path(const char *str);
static char error_message[30] = "An error has occurred\n";
void buildIn_exit(){
    exit(0);
}
void buildIn_cd(const char **args){
    if(args[1]==NULL||args[2]!=NULL){
        write(STDERR_FILENO, error_message, strlen(error_message));
        return ;
    }
    else if(!chdir(args[1])){
        printf("currenttly working directory:%s\n",args[1]);
    }
    else{
        write(STDERR_FILENO, error_message, strlen(error_message));
        return ;
    }
    free(args);
}
void buildIn_path(const char **args){
    if(args[1]==NULL)
    add_path(NULL);
    else{
        char* tmpStr = strdup(args[1]);
        for(int i =2;args[i]!=NULL;++i){
            strcat(tmpStr," ");
            strcat(tmpStr,args[i]);
        }
        add_path(tmpStr);
    
    }
}
int buildIn_try(const char **str){
    //for cd
    if(str==NULL)
    return 0;
    if(str[0][0]=='c'&&str[0][1]=='d')
    buildIn_cd(str);
    else if(str[0][0]=='p'&&str[0][1]=='a'&&str[0][2]=='t'&&str[0][3]=='h')
    buildIn_path(str);
    else if(str[0][0]=='e'&&str[0][1]=='x'&&str[0][2]=='i'&&str[0][3]=='t')
    buildIn_exit();
    else
    return 0;
    return 1;
}
    //try 会判断字符串是不是buildin命令,如果是的话,那就运行,并返回1,否则就返回0