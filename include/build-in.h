void buildIn_exit();
void buildIn_cd(const char **);
void buildIn_path(const char **);
int buildIn_try(const char **);//try 会判断字符串是不是buildin命令,如果是的话,那就运行,并返回1,否则就返回0