
#include "onewire.h"

unsigned char onewire_reset()
{
	CLI;
	unsigned char i;
	ONEWIRE_LOW;
	delay_us(480);
	ONEWIRE_HIGH;
	delay_us(2);

	for (i=0; i<100; i++)
	{
		if (!(ONEWIRE_LEVEL))
		{
			while (!(ONEWIRE_LEVEL));
			SEI;
			return 1;
		}
		delay_us(1);
	}
	SEI;
	return 0;
}

void onewire_send(unsigned char b)
{
	CLI;
	unsigned char temp, i;
	for(i=0; i<8; i++)
	{
		temp=(b&0x01);
		ONEWIRE_LOW;
		if(temp)
		{ 
			delay_us(6);
			ONEWIRE_HIGH;
			delay_us(64);
		}
		else
		{
			delay_us(60);
			ONEWIRE_HIGH;
			delay_us(10);
		}
		b>>=1;
	}
	SEI;
}

unsigned char onewire_read_bit()
{
	CLI;
	unsigned char bit;
	ONEWIRE_LOW;
	delay_us(2);
	ONEWIRE_HIGH;
	delay_us(5);
	bit = ONEWIRE_LEVEL;
	delay_us(80);
	SEI;
	return bit;
}


unsigned char onewire_read()
{
	CLI;
	unsigned byte = 0, i;
	for (i=0; i<8; i++)
	{
		byte >>= 1;
		if (onewire_read_bit())
		byte |= 0x80;
	}
	SEI;
	return byte;
}


int readtt_2(void){
	int temper;
	unsigned char temp[2];
	onewire_reset();
	onewire_send(0xCC);
	onewire_send(0x44);
	//while(!(ONEWIRE_LEVEL));
	delay_ms(20);
	onewire_reset();
	onewire_send(0xCC);
	onewire_send(0xBE);
	
	temp[0]=onewire_read();
	temp[1]=onewire_read();
		
	temper = ((temp[1]<<8)|temp[0]);
	return temper;
		
}
