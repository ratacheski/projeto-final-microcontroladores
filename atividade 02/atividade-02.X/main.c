/*
 * File:   main.c
 * Author: Rafael Ratacheski
 *
 * Created on 6 de Junho de 2021, 21:31
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
#include <stdbool.h>

/*==========================================================================*/
/* Variáveis Globais                                                        */
/*==========================================================================*/
int count = 0;
int belt_position = 1;
float tensao = 0;
char linha1[16];
char linha2[16];

void setup_ADC(void) {
    TRISA = 0b00000001; // Habilita pino A0 como entrada
    ADCON2bits.ADCS = 0b100; // Clock do AD: Fosc/4
    ADCON2bits.ACQT = 0b010; // Tempo de aquisição: 4 Tad
    ADCON2bits.ADFM = 0b1; // Formato: à direita
    ADCON1bits.VCFG = 0b00; // Tensões de referência: Vss e Vdd
    ADCON0bits.CHS = 0b0000; // Seleciona o canal AN0
    ADCON0bits.ADON = 1; // Liga o AD
}

void print_belt(bool stopped) {
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(linha1);
    Lcd_Set_Cursor(2, 1);
    sprintf(linha2, "----------------");
    if (!stopped) {
        linha2[belt_position - 1] = 62;
        if (belt_position == 16) {
            belt_position = 0;
        }
        belt_position++;
    }
    Lcd_Write_String(linha2);
}

void main(void) {
    //pinos d como saída
    TRISD = 0;
    TRISE = 0;
    //Frequência do oscilador interno em 4MHz;
    OSCCON = 0b01100000;
    setup_ADC();
    Lcd_Init();
    sprintf(linha1, "    ESTEIRA     ");
    while (1) { // Inicia loop infinito
        ADCON0bits.GO = 1; // Inicia a conversão A/D
        while (!ADCON0bits.GODONE) { // Aguarda fim da conversão
        }
        count = (ADRESH * 0x100) + ADRESL; // Transfere valor lido para ?contador?
        tensao = ((5 * count) / 1023.0); // Calcula tensão real
        if (tensao == 0) {
            print_belt(true);
        } else {
            print_belt(false);
            if (tensao <= 1) {
                __delay_ms(1000);
            } else if (tensao <= 2) {
                __delay_ms(500);
            } else if (tensao <= 3) {
                __delay_ms(252);
            } else if (tensao <= 4) {
                __delay_ms(126);
            } else if (tensao <= 5) {
                __delay_ms(63);
            }
        }
    }
}
