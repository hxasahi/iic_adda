#include<reg52.h>
#include<intrins.h>
#include<LCD1602.h>
sbit SDA = P2^0;
sbit SCL = P2^1;
sbit RST=P2^4;   //时钟 加上后可以关掉DS1302芯片输出
sbit dula=P2^6;
sbit wela=P2^7;
//bit ADFlag;

void mDelay(unsigned char j)
{
  unsigned int i;
  for(;j>0;j--)
     {
	  for(i=0;i<125;i++)
	     {;}
	  }
}

/*
void Init_Timer1(void)
{
 TMOD |= 0x10;			     
 TH1=0xff;	              //	 Init value  
 TL1=0x00;
 //PT1=1;                 //   优先级    
 EA=1;                    //   interupt enable 
 ET1=1;                   //   enable timer1 interrupt 
 TR1=1;  
  }
*/

void IIC_Start(void)

{
    SDA = 1;
    SCL = 1;
    _nop_();
    SDA = 0;
    _nop_();
    SCL = 0;
}

void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    _nop_();
    SDA = 1;
}

void IIC_Ack(unsigned char ackbit)  //当ackbit为1时，表示单片机对从设备发送来数据的应答

{ //当ackbit为0时，表示主机接收了最后一个字节，因此不再应答，结束通信
    if(ackbit)
    {
        SDA = 0;
    }
    else
    {
        SDA = 1;
    }
    _nop_();
    SCL = 1;
    _nop_();
    SCL = 0;
    SDA = 1;
    _nop_();
}

bit IIC_WaitAck(void)
{
    SDA = 1;
    _nop_();
    SCL = 1;
    _nop_();
    if(SDA)  //在SCL为高电平期间，因为接收设备未将SDA拉低，所以默认未接收到应答，结束IIC通信
    { 
        SCL = 0;
        IIC_Stop();
        return 0;
    }
    else  //接收到应答，返回1，继续下一个数据字节的传输
    {
        SCL = 0;
        return 1;
    }

}

void IIC_SendByte(unsigned char byt)

{
    unsigned char i;
    for(i=0;i<8;i++)
    { 
        if(byt&0x80)
        {
            SDA = 1;
        }
        else
        {
            SDA = 0;
        }
        _nop_();
        SCL = 1;
        byt <<= 1;
        _nop_();
        SCL = 0;
    }
}

unsigned int IIC_RecByte(void)
{
    unsigned int da;
    unsigned char i;

    for(i=0;i<8;i++)
    { 
        SCL = 1;
        _nop_();
        da <<= 1;
        if(SDA)
        da |= 0x01;
        SCL = 0;
        _nop_();
    }

    return da;
}
/*
void init_pcf8591(void)
{
    IIC_Start();

    IIC_SendByte(0x90); ? ?//大家可以看看IAP15F2K61S2的原理图，就知道为什么是0x90了

    IIC_WaitAck();
    IIC_SendByte(0x03); ?//选择ADC通道3
    IIC_WaitAck();
    IIC_Stop();
}

//接收PCF8591转换过的采样电压值

unsigned char adc_pcf8591(void)
{
    unsigned char temp;


    IIC_Start();
    IIC_SendByte(0x91);
    IIC_WaitAck();
    temp = IIC_RecByte();
    IIC_Ack(0);
    IIC_Stop();

    return temp;

}			*/

/*
void Read_AIN1()
{
	IIC_Start();		//IIC总线起始信号							
	IIC_SendByte(0x90); 	//PCF8591的写设备地址		
	IIC_WaitAck();  	//等待从机应答		
	IIC_SendByte(0x01); 	//写入PCF8591的控制字节		
	IIC_WaitAck();  	//等待从机应答						
	IIC_Stop(); 		//IIC总线停止信号					
	
	IIC_Start();		//IIC总线起始信号									
	IIC_SendByte(0x91); 	//PCF8591的读设备地址		
	IIC_WaitAck(); 		//等待从机应答		
	dat = IIC_RecByte();	//读取PCF8591通道3的数据 			
	IIC_Ack(0); 		//产生非应答信号				
	IIC_Stop(); 		//IIC总线停止信号					
}
  */

unsigned int ReadADC()//ReadADC(unsigned char Chl)
{
    unsigned int Data;
	IIC_Start();	   //Start();        //写入芯片地址
	IIC_SendByte(0x90);	 //  Send(AddWr);
	IIC_WaitAck();	 //  Ack();
 	IIC_SendByte(0x01);	   //Send(0x40|Chl);//写入选择的通道，本程序只用单端输入，差分部分需要自行添加
	IIC_WaitAck();	                  //Chl的值分别为0、1、2、3，分别代表1-4通道
	IIC_Stop();	   //Ack();
	
	IIC_Start();	   //Start();
	IIC_SendByte(0x91); //	   Send(AddRd);    //读入地址
	IIC_WaitAck();//	   Ack();
	Data = IIC_RecByte();    //	   Data=Read();    //读数据
	IIC_Ack(0);//	   Scl=0;
	IIC_Stop();//	   NoAck();
	return Data;	   //Stop();
		   //return Data;   //返回值
	  }
	
void clean(void)//数码管锁存函数		   关时钟DS1302
{
    dula=1;
    P0=0x00;
    dula=0;
    wela=1;
    P0=0x00;
    wela=0;
    RST=0;		//		关时钟DS1302
}


void main(){
	
    unsigned char ADtemp;                //定义中间变量
    InitLcd();
	clean();
	//mDelay(20);
    //Init_Timer1();
    while(1){
		ADtemp=ReadADC();		  //对应开发板上的AD1
		//ADtemp=ADtemp*100;
		(int)ADtemp=(ADtemp*500.0)/256;	//*5一位，*50两位，*500三位
		//ADtemp=ADtemp*100;
		TempData[2]=((ADtemp)%100%10);//处理0通道电压显示    -5,经参数调试后发现AD电压范围5V-9.98V,应为0V-5V;
  		TempData[1]=(ADtemp)%100/10;
  		TempData[0]=(ADtemp)/100;

		disp();
        mDelay(40);     //延时用于清晰看出变化
	    //if(ADFlag)      //定时采集输入模拟量	
   		//{
		//ADFlag=0;    
		/*ADtemp=ReadADC();		  //对应开发板上的AD1
		TempData[0]=(ADtemp)/50;//处理0通道电压显示
  		TempData[1]=((ADtemp)%50)/10;
		disp();*/
		//}
    }
}
/*
void Timer1_isr(void) interrupt 3 using 1//定时器1执行数码管动态扫描
{

 static unsigned int j;
 TH1=0xfb;		//重新赋值
 TL1=0x00;
 j++;
 if(j==200) 
    {j=0;ADFlag=1;} //定时置位AD采样标志位
} */