#include"wish.h"
PATH shellPath;
//notice the difference between relative path and absolute path
//path related issues
//input issues:getline,strsep,
//executable issues:fork,execv,wait/waitpid
//check files exists in a directory and is executable :access
void wishInit();
void IM();//interactive mode;
my_mode_t get_mode_t(int ,char*[]);
void add_path(const char*);
int main(int argc,char *argv[]){
    //初始化
    wishInit();
    //模式判断
    my_mode_t MODE = get_mode_t(argc,argv);
    switch(MODE){
        case BATCH_MODE:
        //do sth;
        break;
        case INTER_MODE:
        IM();
        break;
        default:printf("not implemented mode.\n");
    }
}
void wishInit(){
    //初始化添加PATH;
    shellPath.counts=0;
    shellPath.maxSize=0;
    shellPath.path=NULL;
    add_path("/bin");
//    printf("[debug:]%s %d %d\n",shellPath.path[0],shellPath.counts,shellPath.maxSize);

}
void IM()
{
    //interactive mode
    //waiting for build-in command exit
    //只要接受到的不是exit或者是eof,那就不用退出,注意错误处理
    //a infinite loop
}
my_mode_t get_mode_t(int argc,char*argv[]){

}
void add_path(const char *str){
    if(shellPath.path==0){
        shellPath.maxSize=1;
        shellPath.path = malloc(sizeof(char*));
        if(shellPath.path==NULL)
        {
            printf("[shellPath init:]not enough space!!!\n");
            exit(1);
        }
    }
    else 
    if(shellPath.counts+1>shellPath.maxSize)
    {
        //reallocation
        char**tmpPath =realloc(shellPath.path,sizeof(char*)*(shellPath.maxSize*2));
        if(tmpPath==NULL){
            printf("add_path:reallocation failed.\n");
            exit(1);
        }
        else{
            shellPath.path=tmpPath;
            shellPath.maxSize*=2;
        }
    }
    shellPath.path[shellPath.counts++]=strdup(str);
    return ;
}
//11号工作：填完坑,能够完成BATCH,INTERACTIVE的功能
//随后继续看Swapping的部分