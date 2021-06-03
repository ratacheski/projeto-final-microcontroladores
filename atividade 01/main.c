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
unsigned int result = 0;
unsigned int digito = 0;
char caracter;
char linha1[16];
char linha2[16];
char tx_string[30];
__bit flag_interrupcao = 0;

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

void __interrupt() RS232(void) {
    if (RCIF) {
        if (RCREG == '\r') {
            sprintf(linha2, "       ans=%5u", x);
            Lcd_Set_Cursor(2, 1);
            Lcd_Write_String(linha2);

            sprintf(tx_string, "%u\r", x);
            serial_sendString(tx_string);
            x = 0;
        } else {
            digito = RCREG - 0x30;
            x = x * 10 + digito;
            sprintf(linha1, "%5u           ", x);
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String(linha1);
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
    Lcd_Set_Cursor(2, 1);
    Lcd_Write_String("Teste");
    __delay_ms(200);
    serial_sendString("Teste");
    while (1) {
        if (flag_interrupcao == 1) {
            serial_send(caracter); // envia o caractere digitado via serial
            flag_interrupcao = 0;
        }
    }
}
