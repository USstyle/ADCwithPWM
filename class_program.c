/*
 * File:   class_program.c
 * Author: USstyle
 *
 * Created on 28 August, 2023, 7:25 PM
 */


#include <xc.h>
// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

# define _XTAL_FREQ 6000000
void init(void);
void lcdcmd(unsigned char);
void lcddata(unsigned char);

void adcout(unsigned long);
unsigned int lowval,highval,a[10];
unsigned long voltage,value;

void main(void) {
    init();
    while(1)
    {  
        ADCON0=0x81;
       ADCON0|=0x04;//setting godone bit
       while(ADCON0&0x04);
       lowval=ADRESL;
       highval=ADRESH;
       value=((unsigned int)highval<<8)+(unsigned int)lowval;
       voltage=value/3; //1024/3=341
       lcdcmd(0x80);
       adcout(voltage);
       CCPR1L=(voltage)>>2;
       CCP1CON=(CCP1CON&0x0C)+((voltage&0x03)<<4);
       __delay_ms(1000);
    }
    return;
}
void init(void)
{
    TRISD=0x00;//directing as a output
    TRISA=0x01; //RA0 is analog input
    TRISC=0x00; //directing as a output
    ADCON0=0x81;//(1000 0001)
    ADCON1=0x8E;//(1000 1110)
    CCP1CON=0x0C;//(0000 1100)
    CCPR1L=0x00;//(0000 0000)
    T2CON=0x06;//(0000 0110)
    PR2=0x5D;//93
    lcdcmd(0x30);//Function SET
    __delay_ms(100);
    lcdcmd(0x30);//Function Set
    __delay_ms(100);
    lcdcmd(0x30);//Function set
    __delay_ms(100);
    lcdcmd(0x30);//Function set
    __delay_ms(100);
    lcdcmd(0x38);//Function set
    __delay_ms(100);
    lcdcmd(0x06);//Entry mode (increment))
    __delay_ms(100);
    lcdcmd(0x0C);//Display ON
    __delay_ms(100);
    lcdcmd(0x01);//clear display
    __delay_ms(100);
}
void lcdcmd(unsigned char mycmd)
{
    PORTC&=~0x08; //RS=0
    PORTD=mycmd; //giving command
    PORTC|=0x01; //EN=1
    __delay_ms(100);
    PORTC&=~0x01;//EN=0
}
void lcddata(unsigned char mydata)
{
    PORTC|=0x08;//RS=1
    PORTD=mydata;//giving data
    PORTC|=0x01;//EN=1
    __delay_ms(100);
    PORTC&=~0x01;//EN=0
}
void adcout(unsigned long val)
{   
    unsigned char d,j=1;
     while(val!=0)
       {
          d=val%10;
          a[j]=d;
          j++;
          val=val/10;
       }
  a[j]='\0';
  j=j-1;
  if(a[3]>0)
{
lcddata(0x30+a[3]);
}
else
{
lcddata(0x30);
}
  if(a[2]>0)
{
lcddata(0x30+a[2]);
}
else
{
lcddata(0x30);
}
lcddata(0x30+a[1]);
lcddata(0x56);
}