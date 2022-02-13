#include"wish.h"
#include"parser.h"
#include"batch.h"
#include<unistd.h>
char error_message[30] = "An error has occurred\n";
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
        BATCH(argv[1]);
        exit(0);
        break;
        case INTER_MODE:
        IM();
        break;
        default:printf("not implemented mode.\n");
    }
}
void wishInit(){
    //初始化添加PATH;
    shellPath.path=NULL;
    add_path("/bin");
}
void IM()
{
    do{
        printf("wish> ");
        char *str =NULL;
        size_t size=0;
        int return_code=
        getline(&str,&size,stdin);
        //注意getline会读入换行符
        if(str[strlen(str)-1]=='\n')
        str[strlen(str)-1]='\0';
        if(return_code==-1){
            //error fix code:
            break;
        }
        else if(return_code==0){
            continue;
        }
        else{
            //解析命令
            insSet *myIns = parser(str);
            //执行命令
            
            int exec_result = exec(myIns);
            if(exec_result==END_LOOP)
            break;
        }
    }while(1);
    //只要接受到的不是exit或者是eof,那就不用退出,注意错误处理
}
my_mode_t get_mode_t(int argc,char*argv[]){
    if(argc==1)
    return INTER_MODE;
    else if(argc==2)
    return BATCH_MODE;
    else 
    return ERROR_MODE;
}
void add_path(const char *str){
    if(shellPath.path!=NULL){
        //clear
        for(int i =0;shellPath.path[i]!=NULL;++i)
        free(shellPath.path[i]);
        free(shellPath.path);
    }
    shellPath.path=myStrSep(str);
    for(int i =0;shellPath.path!=NULL&&shellPath.path[i]!=NULL;++i)
    printf("%s\n",shellPath.path[i]);
}
