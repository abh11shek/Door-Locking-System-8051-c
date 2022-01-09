#include<reg51.h>
sbit r1 = P2^0;
sbit r2 = P2^1;
sbit r3 = P2^2;
sbit r4 = P2^3;
//
sbit c1 = P2^4;
sbit c2 = P2^5;
sbit c3 = P2^6;
//
sbit motp1 = P3^0;
sbit motp2 = P3^1;
//
sbit rs = P3^5;
sbit rw = P3^6;
sbit en = P3^7;
//
sbit isClosedButton = P0^0;
//
void lcdcmd(unsigned char);
void lcddat(unsigned char);
void lcddis(unsigned char *q);
char keypad();
void check();
void delay(unsigned int);
void invokeDoorUnlock();
void invokeDoorLock();
void initiateDoorLockingProcedure();

unsigned char pin[] = {"12345"};
unsigned char Epin[5];
void main()
{
	isClosedButton = 1;
	lcdcmd(0x0F);
	lcdcmd(0x38);
	lcdcmd(0x01);
	while(1)
	{
		unsigned int i = 0;
		TCON = 0x01;
		IE = 0x81;
		lcdcmd(0x80);
		lcddis("ENTER PIN...");
		delay(1000);
		lcdcmd(0xc0);
		while(pin[i] != '\0')
		{
			Epin[i] = keypad();
			delay(1000);
			i++;
		}
		check();
	}
}

void delay(unsigned int j)
{
	int a, b;
	for(a=0; a<j; a++)
		for(b=0; b<10; b++);
}

void lcdcmd(unsigned char A)
{
	P1 = A;
	rs = 0; rw = 0; en = 1;
	delay(1000);
	en = 0;
}

void lcddat(unsigned char i)
{
	P1 = i;
	rs = 1; rw = 0; en = 1;
	delay(1000);
	en = 0;
}

void lcddis(unsigned char *q)
{
	int k;
	for(k=0; q[k]!='\0'; k++)
		lcddat(q[k]);
	delay(1000);
}

char keypad()
{
	int x = 0;
	while(x == 0)
	{
		r1 = 0; r2 = 1; r3 = 1; r4 = 1;
		if(c1 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '1';
		}
		if(c2 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '2';
		}
		if(c3 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '3';
		}
		r1 = 1; r2 = 0; r3 = 1; r4 = 1;
		if(c1 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '4';
		}
		if(c2 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '5';
		}
		if(c3 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '6';
		}
		r1 = 1; r2 = 1; r3 = 0; r4 = 1;
		if(c1 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '7';
		}
		if(c2 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '8';
		}
		if(c3 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '9';
		}
		r1 = 1; r2 = 1; r3 = 1; r4 = 0;
		if(c1 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '*';
		}
		if(c2 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '0';
		}
		if(c3 == 0)
		{
			lcddat('*');
			delay(100);
			x=1;
			return '#';
		}
	}
}

void check()
{
	if(pin[0]==Epin[0] && pin[1]==Epin[1] && pin[2]==Epin[2] && pin[3]==Epin[3] && pin[4]==Epin[4])
	{
		delay(1000);
		lcdcmd(0x01);
		lcdcmd(0x81);
		lcddis("ACCESS GRANTED");
		delay(1000);
		invokeDoorUnlock();
		lcdcmd(0x01);
		initiateDoorLockingProcedure();
	}
	else
	{
		lcdcmd(0x01);
		lcdcmd(0x80);
		lcddis("INCORRECT PIN...");
		delay(1000000);
		lcdcmd(0x01);
	}
}

void invokeDoorUnlock()
{
	motp1 = 1; motp2 = 0;
	lcdcmd(0xc1);
	lcddis("UNLOCKING...");
	delay(1000000);
	motp1 = 0; motp2 = 0;
}

void invokeDoorLock()
{
	motp1 = 0; motp2 = 1;
	lcdcmd(0x01);
	lcdcmd(0x81);
	lcddis("DOOR CLOSED");	
	lcdcmd(0xc1);
	lcddis("LOCKING...");
	delay(1000000);
	motp1 = 0; motp2 = 0;
	lcdcmd(0x01);
}

void unlockButtonPress() interrupt 0
{
	lcdcmd(0x01);
	lcdcmd(0x81);
	lcddis("ACCESS GRANTED");
	delay(1000);
	invokeDoorUnlock();
	initiateDoorLockingProcedure();
	lcdcmd(0x80);
	lcddis("ENTER PIN...");
	delay(1000);
	lcdcmd(0xc0);	
}

void initiateDoorLockingProcedure()
{
	lcdcmd(0x01);
	lcdcmd(0x81);
	lcddis("DOOR IS OPEN");
	delay(10000000);
	while(isClosedButton);
	invokeDoorLock();
}
