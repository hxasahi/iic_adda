#include<reg52.h>
#include<intrins.h>
#include<LCD1602.h>
sbit SDA = P2^0;
sbit SCL = P2^1;
sbit RST=P2^4;   //ʱ�� ���Ϻ���Թص�DS1302оƬ���
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
 //PT1=1;                 //   ���ȼ�    
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

void IIC_Ack(unsigned char ackbit)  //��ackbitΪ1ʱ����ʾ��Ƭ���Դ��豸���������ݵ�Ӧ��

{ //��ackbitΪ0ʱ����ʾ�������������һ���ֽڣ���˲���Ӧ�𣬽���ͨ��
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
    if(SDA)  //��SCLΪ�ߵ�ƽ�ڼ䣬��Ϊ�����豸δ��SDA���ͣ�����Ĭ��δ���յ�Ӧ�𣬽���IICͨ��
    { 
        SCL = 0;
        IIC_Stop();
        return 0;
    }
    else  //���յ�Ӧ�𣬷���1��������һ�������ֽڵĴ���
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

    IIC_SendByte(0x90); ? ?//��ҿ��Կ���IAP15F2K61S2��ԭ��ͼ����֪��Ϊʲô��0x90��

    IIC_WaitAck();
    IIC_SendByte(0x03); ?//ѡ��ADCͨ��3
    IIC_WaitAck();
    IIC_Stop();
}

//����PCF8591ת�����Ĳ�����ѹֵ

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
	IIC_Start();		//IIC������ʼ�ź�							
	IIC_SendByte(0x90); 	//PCF8591��д�豸��ַ		
	IIC_WaitAck();  	//�ȴ��ӻ�Ӧ��		
	IIC_SendByte(0x01); 	//д��PCF8591�Ŀ����ֽ�		
	IIC_WaitAck();  	//�ȴ��ӻ�Ӧ��						
	IIC_Stop(); 		//IIC����ֹͣ�ź�					
	
	IIC_Start();		//IIC������ʼ�ź�									
	IIC_SendByte(0x91); 	//PCF8591�Ķ��豸��ַ		
	IIC_WaitAck(); 		//�ȴ��ӻ�Ӧ��		
	dat = IIC_RecByte();	//��ȡPCF8591ͨ��3������ 			
	IIC_Ack(0); 		//������Ӧ���ź�				
	IIC_Stop(); 		//IIC����ֹͣ�ź�					
}
  */

unsigned int ReadADC()//ReadADC(unsigned char Chl)
{
    unsigned int Data;
	IIC_Start();	   //Start();        //д��оƬ��ַ
	IIC_SendByte(0x90);	 //  Send(AddWr);
	IIC_WaitAck();	 //  Ack();
 	IIC_SendByte(0x01);	   //Send(0x40|Chl);//д��ѡ���ͨ����������ֻ�õ������룬��ֲ�����Ҫ�������
	IIC_WaitAck();	                  //Chl��ֵ�ֱ�Ϊ0��1��2��3���ֱ����1-4ͨ��
	IIC_Stop();	   //Ack();
	
	IIC_Start();	   //Start();
	IIC_SendByte(0x91); //	   Send(AddRd);    //�����ַ
	IIC_WaitAck();//	   Ack();
	Data = IIC_RecByte();    //	   Data=Read();    //������
	IIC_Ack(0);//	   Scl=0;
	IIC_Stop();//	   NoAck();
	return Data;	   //Stop();
		   //return Data;   //����ֵ
	  }
	
void clean(void)//��������溯��		   ��ʱ��DS1302
{
    dula=1;
    P0=0x00;
    dula=0;
    wela=1;
    P0=0x00;
    wela=0;
    RST=0;		//		��ʱ��DS1302
}


void main(){
	
    unsigned char ADtemp;                //�����м����
    InitLcd();
	clean();
	//mDelay(20);
    //Init_Timer1();
    while(1){
		ADtemp=ReadADC();		  //��Ӧ�������ϵ�AD1
		//ADtemp=ADtemp*100;
		(int)ADtemp=(ADtemp*500.0)/256;	//*5һλ��*50��λ��*500��λ
		//ADtemp=ADtemp*100;
		TempData[2]=((ADtemp)%100%10);//����0ͨ����ѹ��ʾ    -5,���������Ժ���AD��ѹ��Χ5V-9.98V,ӦΪ0V-5V;
  		TempData[1]=(ADtemp)%100/10;
  		TempData[0]=(ADtemp)/100;

		disp();
        mDelay(40);     //��ʱ�������������仯
	    //if(ADFlag)      //��ʱ�ɼ�����ģ����	
   		//{
		//ADFlag=0;    
		/*ADtemp=ReadADC();		  //��Ӧ�������ϵ�AD1
		TempData[0]=(ADtemp)/50;//����0ͨ����ѹ��ʾ
  		TempData[1]=((ADtemp)%50)/10;
		disp();*/
		//}
    }
}
/*
void Timer1_isr(void) interrupt 3 using 1//��ʱ��1ִ������ܶ�̬ɨ��
{

 static unsigned int j;
 TH1=0xfb;		//���¸�ֵ
 TL1=0x00;
 j++;
 if(j==200) 
    {j=0;ADFlag=1;} //��ʱ��λAD������־λ
} */