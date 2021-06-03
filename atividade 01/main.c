/*
 * File:   main.c
 * Author: Rafael Ratacheski
 *
 * Created on 3 de Junho de 2021, 12:30
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
#include <string.h>
#include <stdlib.h>

/*==========================================================================*/
/* Variáveis Globais                                                        */
/*==========================================================================*/
unsigned int x = 0;
unsigned int y = 0;
unsigned int digito = 0;
char linha1[16];
char linha2[16];
char tx_string[30];
__bit hasX = 0;

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

void reset_values(){
    x = 0;
    y = 0;
    hasX = 0;
}

void get_x() {
    if (RCREG == '\r') {
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String("Digite o valor Y");
        sprintf(tx_string, "X = %u\r", x);
        serial_sendString(tx_string);
        hasX = 1;
    } else {
        digito = RCREG - 0x30;
        if (x > 6553 || (x == 6553 && digito > 5)) {
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("Valor  invalido!");
            __delay_ms(200);
            Lcd_Set_Cursor(2, 1);
            Lcd_Write_String("Digite Novamente");
            __delay_ms(500);
            Lcd_Clear();
            x = 0;
        } else {
            x = x * 10 + digito;
            sprintf(linha1, "X = %5u           ", x);
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String(linha1);
        }
    }
}

void get_y_and_sum() {
    if (RCREG == '\r') {
        sprintf(linha2, "   TOTAL = %u", x+y);
        Lcd_Set_Cursor(2, 1);
        Lcd_Write_String(linha2);
        sprintf(tx_string, "Y = %u\r", y);
        serial_sendString(tx_string);        
        sprintf(tx_string, "TOTAL = %u\r", x+y);
        serial_sendString(tx_string);
        reset_values();
    } else {
        digito = RCREG - 0x30;
        if (y > 6553 || (y == 6553 && digito > 5)) {
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("Valor  invalido!");
            __delay_ms(200);
            Lcd_Set_Cursor(2, 1);
            Lcd_Write_String("Digite Novamente");
            __delay_ms(500);
            Lcd_Clear();
            y = 0;
        } else {
            y = y * 10 + digito;
            sprintf(linha1, "Y = %5u           ", y);
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String(linha1);
        }
    }
}

void __interrupt() RS232(void) {
    if (RCIF) {
        if (!hasX) {
            get_x();
        } else {
            get_y_and_sum();
        }
        RCIF = 0;
    }
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
    Lcd_Write_String("      SOMA      ");
    __delay_ms(500);
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("Digite o valor X");
    while (1) {
    }
}
