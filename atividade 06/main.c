/*
 * File:   Atividade 6
 * Author: Joel Felippe Velloso
 *
 * Created on 06 de junho de 2021, 20:30
 */

#define _XTAL_FREQ 4000000

#include <xc.h>

#include <stdio.h>

#define RS LATD2
#define EN LATD3
#define D4 LATD4
#define D5 LATD5
#define D6 LATD6
#define D7 LATD7

#pragma config FOSC = INTOSC_EC
#pragma config WDT = OFF
#pragma config MCLRE = OFF
#pragma config PBADEN = OFF

#include "lcd.h"

int Numero1 = 0;
int Numero2 = 0;
int controle = 0;
int resultado = 0;
int tecla = 0;
char linha1[16];
char linha2[16];
int cont = 1, periodo = 0;
int periodo_anterior = 0;

void setupInt(void) {
  GIE = 1;
  INT0IE = 1;
  INT0IF = 0;
  INTEDG0 = 0; //borda decrescente  
}
void setupTmr0() {
  T08BIT = 0; // Temporizador operando em 16 bits      
  T0CS = 0; // M?dulo operando como temporizador  
  PSA = 1; // Pr?-escala desabilitada   
  TMR0H = 0x3C; // Recarga de 15.535 (conta 50.000 pulsos de 1 us)
  TMR0L = 0xAF;
  TMR0ON = 1; // Liga o temporizador 0     
}

void __interrupt() interrupcao(void) {

  if (TMR0IF) {
      if (periodo > 0) {
        if (cont >= periodo) {
            LATDbits.LATD0 = 0;
            periodo = 0;
            cont = 1;
        } else {
            cont++;
        }
      }
    TMR0H = 0x3C; // Recarga de 15.535 (conta 50.000 pulsos de 1 ?s)
    TMR0L = 0xAF;
    TMR0IF = 0;
    TMR0IE = 1;
  }

  if (INT0IF) {  
    LATDbits.LATD0 = 1;
    periodo = 150;
    cont = 1;
    INT0IF = 0;
  }
}

void main(void) {
  OSCCON = 0b01100000;
  TRISD = 0;
  TRISB = 0x03;
  TRISE = 0x07;
  ADCON1bits.PCFG = 0b1010;

  Lcd_Init();
  setupTmr0();

  Lcd_Set_Cursor(1, 1);
  Lcd_Write_String("Sensor");
  Lcd_Set_Cursor(2, 1);
  Lcd_Write_String("Tempo:  0 s");
  
  setupInt();

  while (1) {      
    if (cont > 1) {
       sprintf(linha2, "Tempo:  %i s", (cont /10));
       Lcd_Set_Cursor(2, 1);
       Lcd_Write_String(linha2);
    } else if (periodo == 0) {
        sprintf(linha2, "Tempo:  0 s ");
       Lcd_Set_Cursor(2, 1);
       Lcd_Write_String(linha2);
    }
    
  }
}