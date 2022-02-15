#include"batch.h"
#include"parser.h"
#include"constants.h"
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<error.h>
static 
char error_message[30] = "An error has occurred\n";
FILE*FP=NULL;
void BATCH(const char* argv){
    FILE* fp = fopen(argv,"r");
    FP=fp;
    if(fp==NULL){
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    char*str=NULL;
    size_t size=0;
    int getline_code;
    while((getline_code=getline(&str,&size,fp))!=-1){
        //注意getline会读入换行符
        if(str[strlen(str)-1]=='\n')
        str[strlen(str)-1]='\0';
        insSet* myIns = parser(str);
        if(exec(myIns)==END_LOOP)
        break;
        free(str);
        str=NULL;
        size_t size=0;
    }
}