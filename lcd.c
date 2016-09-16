/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#include "lcd.h"

void writeFourBits(unsigned char word, unsigned int commandType, 
        unsigned int delayAfter, unsigned int lower){
    
    if(lower){
        LATDbits.LATD0 = word & 0x01;
        LATDbits.LATD1 = (word & 0x02) >> 1;
        LATDbits.LATD2 = (word & 0x04) >> 2;
        LATDbits.LATD3 = (word & 0x08) >> 3;        
    }
    else{
        LATDbits.LATD0 = (word & 0x10) >> 4;
        LATDbits.LATD1 = (word & 0x20) >> 5;
        LATDbits.LATD2 = (word & 0x40) >> 6;
        LATDbits.LATD3 = (word & 0x80) >> 7;                
    }
    
    delayus(delayAfter);
    
    LATDbits.LATD15 = 1; // enable on
    delay(1);
    LATDbits.LATD15 = 0; // enable off
    
}

void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter){
    writeFourBits(word, commmandType, 0); // Send lower 4 bits
    writeFourBits(word, commmandType, 1); // send upper four bits
    delayus(delayAfter);
}

void initLCD(void){
    TRIS_D7 = 0; //map these to pins later.
    TRIS_D6 = 0;
    TRIS_D5 = 0;
    TRIS_D4 = 0;
    TRIS_RS = 0;
    TRIS_E = 0;
         

    // Make pins as outputs
    
    // Set pin to be digital 
    
    // delay 15 milliseconds
    delayus(15000);
    writeFourBits(0x03, LCD_COMMAND, 4100, 1); // delay 4100 microseconds = 4.1 milliseconds
    writeFourBits(0x03, LCD_COMMAND, 100, 1);
    writeFourBits(0x03, LCD_COMMAND, 50, 1); //change to 0 if things don't work
    writeFourBits(0x02, LCD_COMMAND, 40, 1);
    
    writeFourBits(0x02, LCD_COMMAND, 40, 1); //function set
    writeFourBits(0x08, LCD_COMMAND, 40, 1);

    writeFourBits(0x00, LCD_COMMAND, 40, 1);
    writeFourBits(0x08, LCD_COMMAND, 40, 1);//display off

    writeFourBits(0x00, LCD_COMMAND, 40, 1);
    writeFourBits(0x01, LCD_COMMAND, 1640, 1); //clear
    
    writeFourBits(0x00, LCD_COMMAND, 40, 1);
    writeFourBits(0x07, LCD_COMMAND, 40, 1); //entry mode set
    
}



/*
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower);
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter);
void initLCD(void);
void clearLCD(void);
void moveCursorLCD(unsigned char x, unsigned char y);
void printCharLCD(char c);
void printStringLCD(const char* s);
void testLCD();
 */