#include<reg51.h>
sbit rd=P3^7;                  //Read signal P3.7
sbit wr=P3^6;                  //Write signal P3.6
sbit cs=P3^5;                  //Chip Select P3.5
sbit intr= P3^4;                //INTR signal P3.4
int array[10]={0xc0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xf8,0x80,0x90};
sbit dp=P0^7;
sbit led0=P2^0;
sbit led1=P2^1;
sbit led2=P2^2;
unsigned int adc_avg,adc;
void delay()
{
	unsigned int i;
	for(i=1;i<=1000;i++);
}




void main()
{
	char a,b,c;
	cs=0;
	led0=led1=led2=0;
	while(1)
	{
	// Reset tro ve so 0

	//Het reset
  		wr=0;
		wr=1;
		while(intr);
		rd=0;	
		adc=P1;

			adc=adc*10*0.0625;
			a=adc/100;
			b=(adc-a*100)/10;
			c=(adc-a*100-b*10);
			led0=0;
			P0=array[a];
			delay();
			led0=1;
			led1=0;
			P0=array[b];
			dp=0;
			delay();
			led1=1;
			dp=1;
			led2=0;
			P0=array[c];
			delay();
			led2=1;	
		rd=1;
	}
}

