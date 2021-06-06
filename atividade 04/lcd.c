#include "lcd_kit.h"


void atraso_lcd() {
    __delay_ms(3);
}

void instr_wr(char inst) {
    EN = 1;
    RS = 0;
    RW = 0;
    LCD = inst;
    EN = 0;
    atraso_lcd();
}

void dado_wr(char texto) {
    EN = 1;
    RS = 1;
    RW = 0;
    LCD = texto;
    EN = 0;
    atraso_lcd();
}

void inicializa() {
    instr_wr(0x38);
    instr_wr(0x38);
    instr_wr(0x0E);
    instr_wr(0x06);
    instr_wr(0x01);
    initialized = 1;
}

void escreve_texto(char linha[16]) {
    for (int i = 0; i < 16; i++) {
        dado_wr(linha[i]);
    }
}

void clear(char linha[16])
{
    for (int i = 0; i < 16; i++) {
        linha[i] = 0;
    }
}

