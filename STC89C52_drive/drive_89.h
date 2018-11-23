#ifndef _DRIVE_H
#define _DRIVE_H

#include "reg52.h"
#include "intrins.h"
//使用printf会占用额外的1k内存，如果对程序大小敏感请注释掉stdio.h这个库，使用SendString();
#include "stdio.h"

#define FOSC 11059200L          //系统频率
#define BAUD 9600             //串口波特率

//数据类型定义
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

//状态标志类
typedef enum {RESET = 0, SET = !RESET} FlagStatus;
//功能使能类
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;


void delay(int ms);      	
void SerialInit() ;         
void Time0Init();       //1ms系统时钟
void EXIT0Init();       //默认下降沿触发
void EXIT1Init();       //默认下降沿触发
char putchar(char c);
void SendChar(unsigned char ch);
void SendString(unsigned char *str);
uint32_t millis();      //获取定时器计数值

void Time0_Event();     //此函数在主函数定义，用作定时器事件处理函数
void EXIT1_Event();
void EXIT1_Event();
void Time2_Event();
#endif