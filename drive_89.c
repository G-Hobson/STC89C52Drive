#include "drive.h"

uint32_t SysTickCNT=0;

//可自定义定时时间
void Time0Init(uint32_t us)		    
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 =(65536-FOSC/12/us);		//设置定时初值
	TH0 = (65536-FOSC/12/us)>>8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0=1;
}

//定时器2定时1ms初始化，产生系统时钟
void Time2Init()		
{
	T2MOD = 0;		//初始化模式寄存器
	T2CON = 0;		//初始化控制寄存器
	TL2 = (65536-FOSC/12/1000);		//设置定时初值
	TH2 = (65536-FOSC/12/1000)>>8;		//设置定时初值
	RCAP2L = 0x66;  //设置定时重载值
	RCAP2H = 0xFC;	//设置定时重载值
	TR2 = 1;		//定时器2开始计时
}

uint32_t millis()
{
	return SysTickCNT;
}

//1ms不精确延时程序
void delay(unsigned int ms)      //@11.0592MHz		12T模式
{
    unsigned char i, j;

    while(ms--)
    {
        i = 2;
        j = 239;
        do
        {
            while (--j);
        } while (--i);
    }
}

/*串口初始化，默认配置波特率为9600，系统时钟为11.0592MHZ*/
void SerialInit()          //115200bps	@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
    TL1 = (FOSC/12/32/BAUD);
	TH1 =TL1;		    //设定定时器重装值
	ET1 = 0;		    //禁止定时器1中断
	TR1 = 1;		    //启动定时器1
    ES = 1;             //开启串口中断
}

void SendChar(unsigned char ch)
{
    SBUF=ch;
    while(!TI);
    TI=0;
}

void SendString(unsigned char *str)
{
    while(*str!='\0')
        {
            SendChar(*str);
            str++;
        }
}

//重写putchar函数,printf重定向
char putchar(char c)
{
    ES=0;           //关串口中断
    SBUF=c;           
    while(TI!=1);   //等待发送成功
    TI=0;           //清除发送中断标志
    ES=1;           //开串口中断
    return c;
}

void EXIT0Init()
{
	EX0=1;		//开启外部中断
	IT0=1;		//下降沿触发
    EA=0;
}

void EXIT1Init()
{
	EX1=1;		//开启外部中断
	IT1=1;		//下降沿触发
    EA=0;
}

void EXIT0_Handle() interrupt 0
{
    EXIT0_Event();
}

void Time0_Handle() interrupt 5
{
    Time0_Event();
}

void EXIT1_Handle() interrupt 2
{
    EXIT1_Event();
}

void SerialRec(void) interrupt 4
{
	static int location=0;
	char buff;
	if(RI == 1)         //当串口接收到数据时，RI置位
	{
		RI = 0;         //清空接收标志位
        buff = SBUF;
		if(buff!=0x0a)
		{
			RecBuff[location++]=buff;
		}
		else
		{
			RecBuff[location++]=0x0a;
			RecBuff[location]=0x00;

			SerialRecFlag=SET;
			location=0;
		}
	}
}

void Time2_Handle() interrupt 5
{
	TF2 = 0;		//清除TF2标志
    SysTickCNT++;
    Time2_Event();
}

