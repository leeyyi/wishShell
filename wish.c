#include"wish.h"
//notice the difference between relative path and absolute path
//path related issues
//input issues:getline,strsep,
//executable issues:fork,execv,wait/waitpid
//check files exists in a directory and is executable :access
void wishInit();
void IM();//interactive mode;
mode_t get_mode_t(int ,char*[]);
int main(int argc,char *argv[]){
    //初始化
    wishInit();
    //模式判断
    mode_t MODE = get_mode_t(argc,argv);
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
}
void IM()
{
    //interactive mode
    //waiting for build-in command exit
    //只要接受到的不是exit或者是eof,那就不用退出,注意错误处理
    //a infinite loop
}
mode_t get_mode_t(int argc,char*argv[]){

}
//11号工作：填完坑,能够完成BATCH,INTERACTIVE的功能
//随后继续看Swapping的部分