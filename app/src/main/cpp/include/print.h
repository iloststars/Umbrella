#pragma once

enum printMode{black,console,local,remote};
//enum printMode mode = console;
enum printMode mode = black;

void print(const char* __fmt,...);


void print(const char* __fmt,...){
    switch (mode)
    {
        //控制台输出
        case console:
            va_list args;
            va_start(args,__fmt);
            vprintf(__fmt,args);
            va_end(args);
            printf("\n");
            break;
            //本地文件输出
        case local:
            break;
            //远程服务器输出
        case remote:

            break;
        case black:
            break;
    }
}