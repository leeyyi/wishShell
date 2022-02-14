#include"parser.h"
#include"PATH.h"
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include"build-in.h"
#define defaultSize 10
extern FILE* FP;
extern PATH shellPath;
struct __insSet{
    char ***args;
};
insSet *parser(const char*Str){
    //初代版本,之后的版本应该支持指令并行
    insSet* ins = malloc(sizeof(insSet));
    if(ins==NULL)
    {
        printf("error occurred in parser:malloc.\n");
        exit(1);
    }
    ins->args=malloc(sizeof(char**)*defaultSize);//最开始只支持10条并行语句
    ins->args[0]=myStrSep(Str);
    ins->args[1]=NULL;
    return ins;
}
void afterExec(insSet* ins){
    if(ins->args!=0)
    for(int i =0;ins->args[i]!=NULL;++i){
        if(ins->args[i]!=0)
        for(int j =0;ins->args[i][j]!=NULL;++j){
            free(ins->args[i][j]);
        }
        free(ins->args[i]);
    }
    free(ins->args);
    free(ins);
}
int exec(insSet*ins){
    //对于每一条指令
    if(ins->args!=0)
    for(int i =0;ins->args[i]!=NULL;++i){
        int absoluteOK=1;//为0 是相对路径
        char *tmpAbs=NULL;
        if(ins->args[i][0]==NULL)
        continue;
        if(buildIn_try(ins->args[i]))
        continue;
        if(ins->args[i][0][0]!='/'&&ins->args[i][0][0]!='.'){
            for(int j =0;shellPath.path[j]!=NULL;++j){
                tmpAbs = malloc(sizeof(char)*(strlen(shellPath.path[j])+strlen(ins->args[i][0])+1));
                strcpy(tmpAbs,shellPath.path[j]);
                strcat(tmpAbs,"/");
                strcat(tmpAbs,ins->args[i][0]);
                if((absoluteOK=access(tmpAbs,X_OK))==0)
                {
                    break;
                }
                free(tmpAbs);
                tmpAbs=NULL;
            }
        }
        if(absoluteOK){
            if(access(ins->args[i][0],X_OK)==0)
            {
            }
            else{
                //printf("the file is not existed.\n");
            }
        }
        else{
            //修正执行参数
            free(ins->args[i][0]);
            ins->args[i][0]=tmpAbs;
        }
        int redirec=-1;//如果后续增加循环执行,应该每一轮循环都把redirec重置-1
        for(int findRed=0;ins->args[i][findRed]!=NULL;++findRed){
            if(strcmp(ins->args[i][findRed],">")==0){
                redirec=findRed;
                break;
            }
        }
        pid_t child = fork();
        if(child>0){
            //parent process
            waitpid(child,NULL,WUNTRACED|WCONTINUED);
            return 0;
        }
        else if(child==0){
            //child process
            fflush(stdout);
            int fd = -1;
            if(redirec!=-1){
                if(close(STDOUT_FILENO)){
                    printf("error occurred in close stdout.\n");
                }
                else 
                if(ins->args[i][redirec+1]==NULL||(fd=open(ins->args[i][redirec+1],O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))==0){
                    fprintf(stderr,"error occurred in open new file stream.\n");
                    if(fd==-1||fd==0)
                    close(fd);
                    _exit(1);
                }
                else{
                    ins->args[i][redirec]=NULL;
                }
            }
            if(execv(ins->args[i][0],ins->args[i])==-1){
                if(fd==-1||fd==0)
                close(fd);
                printf("[error]occurred in execv.\n");
                _exit(1);
            }
        }
        else {
            printf("[exec:fork]error occurs.\n");
            exit(1);
        }
    }
    //执行完指令之后,要把所有内存都free掉
    afterExec(ins);
}
char**myStrSep(const char*str){
    char **argv = malloc(sizeof(char*)*(defaultSize+1));//默认只有十个参数
    if(argv==0){
        printf("error occurred in myStrSep malloc.\n");
        exit(1);
    }
    int totalArg=0;
    int startOfArg=0;
    int inWord=0;
    int remaining=defaultSize;
    int i ;
    if(str!=NULL)
    for(i =0;str[i]!='\0';++i){
        if(str[i]!=' '){
            if(inWord)
            continue;
            else {
                startOfArg=i;
                inWord=1;
            }
        }
        else{
            if(inWord){
                if(remaining==0){
                    //reallocation
                    char** tmp = realloc(argv,(totalArg*2+1)*sizeof(char*));
                    if(argv==NULL){
                        printf("error occurred in myStrSep :malloc.\n");
                        exit(1);
                    }
                    argv=tmp;
                    remaining=totalArg;
                }
                //复制str;
                argv[totalArg++]=strndup(&str[startOfArg],i-startOfArg);
                remaining--;
                inWord=0;
            }
            else{
                continue;
            }
        }
    }
    //处理最后一个单词
    if(inWord){
        argv[totalArg++]=strndup(&str[startOfArg],i-startOfArg);
    }
    argv[totalArg]=0;
    return argv;
}