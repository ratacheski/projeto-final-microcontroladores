//LCD Functions Developed by electroSome

#define _XTAL_FREQ 20000000

#include <xc.h>

#define EN LATEbits.LATE1
#define RS LATEbits.LATE0
#define RW LATEbits.LATE2

#define LCD LATD

static unsigned char initialized;

void atraso_lcd();

void instr_wr(char inst);

void dado_wr(char texto);

void inicializa();

void escreve_texto(char linha[16]);

void clear();