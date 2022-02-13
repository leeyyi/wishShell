#ifndef __PARSER_H_
#define __PARSER_H_
typedef struct __insSet insSet;
insSet *parser(const char*);
int exec(insSet*);
char**myStrSep(const char*str);
#endif