#include"parser.h"
#include"PATH.h"
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#define defaultSize 10
extern PATH shellPath;
struct __insSet{
    char**instructions;
    char ***args;
};
insSet *parser(const char*Str){
    char*str = strdup(Str);
    int maxArgc=defaultSize;//初始最多只有十个参数
    //第一个版本的解析,以后支持单语句多命令
    insSet *tmp =malloc(sizeof(insSet));
    if(tmp==NULL){
        printf("[parser]:not enough space!!!.\n");
        exit(1);
    }
    //初代版本就假装只有一个命令
    //初始化
    int numOfIns=1;
    tmp->instructions=malloc(sizeof(char*)*(numOfIns+1));
    if(tmp->instructions==NULL){
        printf("[parser]:not enough space!!!.\n");
        exit(1);
    }
    tmp->args=malloc(sizeof(char**)*(numOfIns+1));
    //填写内容
    //第一个单词是命令
    short int isSet=-1;
    short int endOfStr;
    for(int i =0;str[i]!='\0';++i){
        if(str[i]==' '){
            if(isSet==-1)
            continue;
            else{
                str[i]='\0';
                endOfStr=i;
                break;
            }
        }
        if(isSet==-1)
        isSet=i;
    }
    if(isSet==-1){
        tmp->instructions[0]=NULL;//只输入了空格
        return tmp;
    }
    else{
        tmp->instructions[0]=strdup(&str[isSet]);
        tmp->instructions[1]=NULL;
        //填写参数
        int totalSize=0;
        tmp->args[0]=malloc(sizeof(char*)*(maxArgc+1));
        if(tmp->args[0]==NULL){
            printf("[parser:]not enough space!!!.\n");
            exit(1);
        }
        int i ;
        int current=0;//0代表当前是剔除空格,1代表当前扫描单词
        int startOfArg;
        int totalArg=0;
        for(i = endOfStr+1;str[i]!='\0';++i){
            if(current==0)
            {
                if(str[i]==' ')
                continue;
                else 
                {
                    current=1;
                    startOfArg=i;
                }
            }
            else{
                if(str[i+1]=='\0'){
                    tmp->args[0][totalArg++]=strdup(&str[startOfArg]);
                }
                else if(str[i+1]==' '){
                    str[i+1]='\0';
                    tmp->args[0][totalArg++]=strdup(&str[startOfArg]);
                    i=i+2;
                }
                
            }
        }
        tmp->args[0][totalArg]=NULL;
    }
    free(str);
    return tmp;
}
void afterExec(insSet* ins){
    for(int i =0;ins->instructions[i]!=NULL;++i){
        free(ins->instructions[i]);
        for(int j =0;ins->args[i][j]!=NULL;++j){
            free(ins->args[i][j]);
        }
        free(ins->args[i]);
    }
    free(ins->instructions);
    free(ins->args);
    free(ins);
}
int exec(insSet*ins){
    //对于每一条指令
    for(int i =0;ins->instructions[i]!=NULL;++i){
        int absoluteOK=1;//为0 是相对路径
        char *tmpAbs=NULL;
        if(ins->instructions[i][0]!='/'&&ins->instructions[i][0]!='.'){
            for(int j =0;j<shellPath.counts;++j){
                tmpAbs = malloc(sizeof(char)*(strlen(shellPath.path[j])+strlen(ins->instructions[i])+1));
                strcpy(tmpAbs,shellPath.path[j]);
                strcat(tmpAbs,"/");
                strcat(tmpAbs,ins->instructions[i]);
                if((absoluteOK=access(tmpAbs,X_OK))==0)
                {
                    break;
                }
                perror(NULL);
                free(tmpAbs);
                tmpAbs=NULL;
            }
            
        }
        char*tmpExec;
        if(absoluteOK){
            if(access(ins->instructions[i],X_OK)==0)
            {
                tmpExec=ins->instructions[i];
            }
            else{
                printf("the file is not existed.\n");
            }
        }
        else{
            //exec tmpAbs;
            tmpExec=tmpAbs;
        }
        pid_t child = fork();
        if(child>0){
            //child process
            execv(tmpExec,ins->args[i]);
        }
        else if(child==0){
            //parent process
            waitpid(child,NULL,WUNTRACED|WCONTINUED);
            return 0;
        }
        else {
            printf("[exec:fork]error occurs.\n");
            exit(1);
        }
        free(tmpAbs);
    }
    //执行完指令之后,要把所有内存都free掉
    afterExec(ins);
}