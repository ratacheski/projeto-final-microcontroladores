/* 
 * File:   main.c
 * Author: renan
 *
 * Created on 5 de Junho de 2021, 14:22
 */
#define _XTAL_FREQ 20000000

#include "Game.h"

#pragma config FOSC = INTOSC_EC 
#pragma config WDT  = OFF
#pragma config MCLRE = OFF   
#pragma config PBADEN = OFF

void setupInt(void) {
    GIE = 1;  

    INT0IE = 1;    
    INT0IF = 0;
    INTEDG0 = 1;    

    INT1IE = 1;  
    INT1IF = 0;    
    INTEDG1 = 1;  

    INT2IE = 1;  
    INT2IF = 0;    
    INTEDG2 = 1;  
    
    TMR0IE = 1; 
    TMR0IF = 0;
 }

void setupTmr0() {
    T08BIT = 0;             // Temporizador operando em 16 bits      
    T0CS = 0;        // Módulo operando como temporizador  
    PSA = 1;           // Pré-escala desabilitada   
    TMR0H = 0x3C;    // Recarga de 15.535 (conta 50.000 pulsos de 1 us)
    TMR0L = 0xAF;    
    TMR0ON = 1;           // Liga o temporizador 0     
}



void __interrupt() interrupcao(void) {
    if (INT0F){
        if (playing != 1){
            initGame();
        }
        else{
            checkPlayerInput(0);
        }
        INT0F = 0;
    }
    if (INT1F){
        if (playing != 1){
            initGame();
        }
        else{
            checkPlayerInput(1);
        }
        INT1F = 0;
    }
    if (INT2F){
        if (playing != 1){
            initGame();
        }
        else{
            checkPlayerInput(2);
        }
        INT2F = 0;
    }
    if (TMR0IF){
        if(cont == 255){
            cont = 0;
        }
        else{
            cont++;
        }
        TMR0H = 0x3C;        // Recarga de 15.535 (conta 50.000 pulsos de 1 ?s)
        TMR0L = 0xAF;
        TMR0IF = 0;
    }
}

int main(int argc, char** argv) {
    OSCCON = 0b01100000;         // Oscilador interno de 4 MHz      
    TRISD = 0x00;
    TRISE = 0x00;
    TRISB = 0b00000111;         
    LATD = 0b11111111;
    __delay_ms(500);
    playing = 0;
    cont = 0;
    EEPROM_WriteByte(0, 0);
    setupInt();
    setupTmr0();
    printStartMessage();
    while(1) {
            
    }

}

