#include "Game.h"
#include "lcd_kit.h"

char linha1[16];
char linha2[16];

void EEPROM_WriteByte(byte eepromAddress, byte eepromData)
{
    unsigned char gie_Status;
    
    while(WR);            // check the WR bit to see if a previous Write operation is in progress
    EEADR=eepromAddress;  // Write the address to EEADR.
    EEDATA=eepromData;    // load the 8-bit data value to be written in the EEDATA register.
    WREN=1;               // Set the WREN bit to enable eeprom operation.
    gie_Status = GIE;     // Copy the current Interrupt state
    GIE = 0;              // Disable the interrupts
    EECON2=0x55;          // Execute the special instruction sequence
    EECON2=0xaa;          // Refer the datasheet for more info
    WR=1;                 // Set the WR bit to trigger the eeprom write operation.
    GIE = gie_Status;     // Restore the interrupts
    WREN=0;               // Disable the EepromWrite
}

byte EEPROM_ReadByte(byte eepromAddress)
{
    while(RD || WR);           // check the WR&RD bit to see if a RD/WR is in progress
    EEADR = eepromAddress;       // Write the address to EEADR.
    RD = 1;                    // Set the RD bit to trigger the eeprom read operation.
    return(EEDATA);            // Return the data read form eeprom.
}

void printStartMessage(){
    if (!initialized)
        inicializa();
    clear(linha1);
    clear(linha2);
    byte record = EEPROM_ReadByte(0);
    sprintf(linha1, "Recorde: %d", record);
    instr_wr(0x80);
    escreve_texto(linha1);
    sprintf(linha2, "Aperte p Iniciar");
    instr_wr(0xC0);
    escreve_texto(linha2); 
}

void printAttentionMessage(){
    clear(linha1);
    clear(linha2);
    sprintf(linha1, "Atencao");
    instr_wr(0x80);
    escreve_texto(linha1);
    sprintf(linha2, "");
    instr_wr(0xC0);
    escreve_texto(linha2);
}

void printPlayingMessage(byte position, byte total){
    clear(linha1);
    clear(linha2);
    sprintf(linha1, "Sua vez");
    instr_wr(0x80);
    escreve_texto(linha1);
    sprintf(linha2, "%d de %d", position, total);
    instr_wr(0xC0);
    escreve_texto(linha2);
}

void printErrorMessage(){
    clear(linha1);
    clear(linha2);
    sprintf(linha1, "Voce errou :(");
    instr_wr(0x80);
    escreve_texto(linha1);
    sprintf(linha2, "");
    instr_wr(0xC0);
    escreve_texto(linha2);
    __delay_ms(500);
}

void printSucessMessage(){
    clear(linha1);
    clear(linha2);
    sprintf(linha1, "Sucesso :)");
    instr_wr(0x80);
    escreve_texto(linha1);
    sprintf(linha2, "");
    instr_wr(0xC0);
    escreve_texto(linha2);
    __delay_ms(500);
}

void printWinnerMessage(){
    clear(linha1);
    clear(linha2);
    sprintf(linha1, "Voce ganhou :)");
    instr_wr(0x80);
    escreve_texto(linha1);
    sprintf(linha2, "");
    instr_wr(0xC0);
    escreve_texto(linha2);
    __delay_ms(500);
}

void generateGameSequence(){  
    byte i;
    int toc = cont;    
    srand(toc);
    for(i = 0 ; i < MAX ; i++){
        gameSequence[i] = (rand() % 3);
    }
}

void showGameSequence(){
    for (byte i = 0; i < level; i++){
        LATD = 0b11111111 ^ (1 << gameSequence[i]);
        __delay_ms(800);
        LATD = 0b11111111;
        __delay_ms(200);
    }
}

void initGame(){
    LATD = 0b11111111;
    playing = 1;
    level = 1;
    playerPosition = 0;
    generateGameSequence();
    printAttentionMessage();
    showGameSequence();
    printPlayingMessage(1, level);
}

void checkPlayerInput(byte option){
    if (option == gameSequence[playerPosition]){
        playerPosition ++;
        if (playerPosition == MAX){
            printSucessMessage();
            printWinnerMessage();
            byte record = EEPROM_ReadByte(0);
            if (MAX > record)
                EEPROM_WriteByte(0, MAX);
            playing = 0;
            printStartMessage();
        }
        else if (playerPosition == level){
            level++;
            playerPosition = 0;
            printSucessMessage();
            printAttentionMessage();
            showGameSequence();
            printPlayingMessage(playerPosition + 1, level);
        }
        else{
            printPlayingMessage(playerPosition + 1, level);
        }
    }
    else{
        printErrorMessage();
        byte record = EEPROM_ReadByte(0);
        if (level - 1 > record)
            EEPROM_WriteByte(0, level - 1);
        playing = 0;
        printStartMessage();
    }
}
