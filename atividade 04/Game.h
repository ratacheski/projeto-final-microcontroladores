/* 
 * File:   Game.h
 * Author: renan
 *
 * Created on 5 de Junho de 2021, 15:18
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef GAME_H
#define	GAME_H

#ifdef	__cplusplus
extern "C" {
#endif
    #define MAX 25
    //Type Bye
    typedef unsigned char byte;
    
    //Global variables
    byte gameSequence[MAX];
    byte playerPosition;
    byte playing;
    byte level;
    byte cont;
    
    //Game Functions
    void EEPROM_WriteByte(byte eepromAddress, byte eepromData);
    
    byte EEPROM_ReadByte(byte eepromAddress);
    
    void printStartMessage();

    void printAttentionMessage();

    void printPlayingMessage(byte position, byte total);

    void printErrorMessage();

    void printSucessMessage();

    void generateGameSequence();

    void showGameSequence();

    void initGame();

    void checkPlayerInput(byte option);

#ifdef	__cplusplus
}
#endif

#endif	/* GAME_H */

