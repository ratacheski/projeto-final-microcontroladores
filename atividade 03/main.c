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
#define _XTAL_FREQ 4000000

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

/*==========================================================================*/
/* Variáveis Globais                                                        */
/*==========================================================================*/
char tx_string[16];

int h = 0;
int m = 59;
int s = 57;
int counter = 0; // Multiplicador do tick de 10ms para obter 1 segundo;
long contagem = 10000; // Variável que define quantos us serão contados a cada conversão
char* h0;
char* m0;
char* s0;

// funcao para contabilizar HH:MM:SS

void cronometro() {
    s = s + 1;
    if (s == 60) {
        s = 0;
        m = m + 1;
    }
    if (m == 60) {
        m = 0;
        h = h + 1;
    }
}

void disp_cronometro() //Função para exibir valor do cronômetro
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
    sprintf(tx_string, "    %s%u:%s%u:%s%u    ", h0, h, m0, m, s0, s);
    Lcd_Set_Cursor(2, 1);
    Lcd_Write_String(tx_string); //Imprimi cronometro no display

}

void __interrupt() timer(void) {
    if (TMR0IF) {
        TMR0H = (0xFFFF - contagem) >> 8; // Cálculo do valor inicial do TMR3
        TMR0L = ((0xFFFF - contagem) & 0xFF); // Cálculo do valor inicial do TMR3
        if (counter == 75) {
            disp_cronometro();
            counter = 0;
        } else {
            counter++;
        }
        TMR0IF = 0; // Limpa a Flag da interrupção
    }
}

void setup_tmr0() {
    T08BIT = 0;
    T0CS = 0; // Fonte do clock
    PSA = 1;
    TMR0H = (0xFFFF - contagem) >> 8; // Cálculo do valor inicial do TMR0
    TMR0L = ((0xFFFF - contagem) & 0xFF); // Cálculo do valor final do TMR0
    TMR0ON = 1; // Liga o timer
}

void setup_interruptions() {
    GIE = 1;
    TMR0IE = 1;
}

void main(void) {
    //pinos d e e como saída
    TRISD = 0;
    TRISE = 0;
    //Frequência do oscilador interno em 4MHz;
    OSCCON = 0b01100000;
    setup_interruptions();
    setup_tmr0();
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("     TIMER      ");
    while (1) {
    }
}