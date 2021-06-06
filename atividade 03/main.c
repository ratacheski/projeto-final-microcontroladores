/*
 * File:   main.c
 * Author: Eduardo Rodrigues de Faria
 *
 * Created on 6 de Junho de 2021, 12:30
 */

/*==========================================================================*/
/* Cronfiguraçao do Cristal                                                 */
/*==========================================================================*/
#include <xc.h>
#define _XTAL_FREQ 400000 // valor para que o tick do timer seja de 10us.

/*==========================================================================*/
/* Pinos do LCD                                                             */
/*==========================================================================*/
#define RS LATE0
#define EN LATE1
#define D4 LATD4
#define D5 LATD5
#define D6 LATD6
#define D7 LATD7

/*==========================================================================*/
/* Cronfigurações gerais                                                    */
/*==========================================================================*/
#pragma config FOSC = INTOSC_EC //oscilador interno
#pragma config WDT = OFF        //Watchdog timer desligado
#pragma config MCLRE = OFF      //Reset desabilitado

#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*==========================================================================*/
/* Variáveis Globais                                                        */
/*==========================================================================*/
char tx_string[30];

int h = 0;
int m = 0;
int s = 0;

char* h0;
char* m0;
char* s0;

void serial_send(char character) {
    TXIF = 0;
    TXREG = character;
    while (TXIF == 0);
}

void serial_sendString(const char phrase[]) {
    unsigned int index = 0;
    unsigned int size = strlen(phrase);
    while (index < size) {
        serial_send(phrase[index]);
        index++;
    }
}


// funcao para contabilizar HH:MM:SS
void cronometro()
{
    s = s+1; 
      
    if (s == 60 ){
        s = 0;
        m = m + 1;
    }
      
      if (m == 60){
          m = 0;
          h = h + 1;
      }

} 

void disp_cronometro()    //Função para exibir valor do cronômetro
{
   cronometro();
     //Atribui o valor das variáveis do relógio aos elementos da string
   h0 = m0 = s0 = "";
   
   if (h < 10)
       h0 = "0";
   
   if (m < 10)
       m0 = "0";
   
   if (s < 10)
       s0 = "0";
       
    sprintf(tx_string, "%s%u:%s%u:%s%u\r", h0, h, m0, m, s0, s);
     
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(tx_string);                        //Imprimi cronometro no display

} //end disp cronometro

void __interrupt() RS232(void) {
    
    cronometro(); //contabiliza HH:MM:SS 
    
    sprintf(tx_string, "%u:%u:%u\r", h,m,s);//Atribui o valor das variáveis do relógio aos elementos da string
     
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(tx_string);
        
}

void setup_RS232() {
    TXEN = 1; // Habilita TX
    SYNC = 0; // Modo Assíncrono
    BRGH = 1; // Baud-rate alto
    SPEN = 1; // Define pinos RX/DT e TX/CK da porta D como RX e TX do serial
    CREN = 1; // Habilita recepção contínua
    BAUDCON = 0x00;
    SPBRG = 25;
}

void setup_Interruptions() {
    GIE = 1;
    PEIE = 1;
    RCIE = 1;
}

void main(void) {
    //pinos d e e como saída
    TRISD = 0;
    TRISE = 0;
    //Frequência do oscilador interno em 4MHz;
    OSCCON = 0b01100000;
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
    
    setup_RS232();
    setup_Interruptions();
    
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(" TIMER ");
    __delay_ms(1000);
    
    Lcd_Clear();
    
    while (1) {
        
      disp_cronometro(); //chama função para mostrar cronômetro
      
    }
}