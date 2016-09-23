/*
 * File:   lcd.c
 * Authors: Alex Brooks, Dallon Lines, Nathan Yokley
 *
 * Created on December 31, 2014, 1:39 PM
 */

/* TODO: Make define statements for each pin used in the LCD
 */
#include <xc.h>
#include "lcd.h"
#include "timer.h"
#include <string.h>
/* The pins that you will use for the lcd control will be
 * LCD_RS      RC4
 * LCD_E       RC2
 * LCD_D4      RE7
 * LCD_D5      RE5
 * LCD_D6      RE3
 * LCD_D7      RE1
 * Use these so that we will be able to test your code for grading and to
 * help you debug your implementation!
 */


//<<<<all data values just given to shut up warnings. Assign actual reg values later.>>>>
#define LCD_DATA  1
#define LCD_RS  TRISCbits.TRISC4
#define LCD_E   TRISCbits.TRISC2

#define TRIS_D7 TRISEbits.TRISE1
#define TRIS_D6 TRISEbits.TRISE3
#define TRIS_D5 TRISEbits.TRISE5
#define TRIS_D4 TRISEbits.TRISE7

#define LCD_D7 LATEbits.LATE1
#define LCD_D6 LATEbits.LATE3
#define LCD_D5 LATEbits.LATE5
#define LCD_D4 LATEbits.LATE7


#define LCD_COMMAND 0
#define LCD_DATA 1

#define LOWER 1
#define UPPER 0

#define INPUT 1
#define OUTPUT 0
/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) {
//    Not totally sure what he meant by be clever with pointers. These codes 
//    are equivalent, but the one below is easier to read. Optimization due to
//    pointer arithmetic vs arrays is minimal, so pick either I guess?
//    char* addr = s;
//    while(*addr != '\0')
//    {
//        printCharLCD(*addr);//nothing else needed since the cursor automatically updates!
//        addr++;
//    }
    int i;
    for(i = 0; i < strlen(s); i++)
        printCharLCD(s[i]); 
    
}
    

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) {
    writeLCD(c, LCD_DATA, 40); //SET RS to LCD_DATA, or 1.
}



/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter){
    writeFourBits(word, commandType, delayAfter, UPPER); // Send upper 4 bits
    writeFourBits(word, commandType, delayAfter, LOWER); // send lower four bits & delay after the write.
}
   


/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATE. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower){
    LCD_RS = commandType == LCD_DATA ? 1 : 0; //only enable LCD_RD if commandType is LCD_DATA.
    if(lower){
        LCD_D4 = word & 0x01; //aka LATDbits.LARTx = (word & F0)
        LCD_D5 = (word & 0x02) >> 1;
        LCD_D6 = (word & 0x04) >> 2;
        LCD_D7 = (word & 0x08) >> 3;        
    }
    else{
        LCD_D4 = (word & 0x10) >> 4; // aka LATDbits.LATDx = (word & F0) >> 4
        LCD_D5 = (word & 0x20) >> 5;
        LCD_D6 = (word & 0x40) >> 6;
        LCD_D7 = (word & 0x80) >> 7;                
    }
    
    delayUs(delayAfter);
    LCD_E = 1; //enable on
    delayUs(1); //IMPORTANT. How long are we supposed to delay this? Not specified in lecture.
    LCD_E = 0; //enable off
}



/*Initialize the LCD
 */
void initLCD(void) {
    // Setup D, RS, and E to be outputs (0).
    
    
    TRIS_D7 = OUTPUT; 
    TRIS_D6 = OUTPUT;
    TRIS_D5 = OUTPUT;
    TRIS_D4 = OUTPUT;
    LCD_RS = OUTPUT;
    LCD_E = OUTPUT;
    
    //MAKE ANALOG IF NEEDED. CHECK IF ANY OF THE PINS USED ARE ANSELx REGISTERS!
    //NOTE: MINIMUM DELAY IS 40 USEC!
    
    
    // delay 15 milliseconds
    delayUs(15000);
    
    writeFourBits(0x03, LCD_COMMAND, 40, LOWER); // delay 4100 microseconds = 4.1 milliseconds
    delayUs(4100);
    
    writeFourBits(0x03, LCD_COMMAND, 40, LOWER);
    delayUs(100);
    
    writeFourBits(0x03, LCD_COMMAND, 40, LOWER); 

    //Function set: 4 bit starts right below this comment.
    writeFourBits(0x02, LCD_COMMAND, 40, LOWER);

    //writeLCD(0x28, LCD_COMMAND, 40);    
    writeFourBits(0x02, LCD_COMMAND, 40, UPPER); //function set operations. 0x28? fix.
    writeFourBits(0x08, LCD_COMMAND, 40, LOWER); 

    
    //writeLCD(0x08, LCD_COMMAND, 40);
    writeFourBits(0x00, LCD_COMMAND, 40, UPPER);
    writeFourBits(0x08, LCD_COMMAND, 40, LOWER);//display off

    //writeLCD(0x01, LCD_COMMAND, 40);
    writeFourBits(0x00, LCD_COMMAND, 40, UPPER); //needed?
    writeFourBits(0x01, LCD_COMMAND, 40, LOWER); //clear display. Lag ~1.64 ms before moving on.
    delayUs(1640); //delay for the clear.

    //writeLCD(0x06, LCD_COMMAND, 40);    
    writeFourBits(0x00, LCD_COMMAND, 40, UPPER); //needed?
    writeFourBits(0x06, LCD_COMMAND, 40, LOWER); //entry mode set. Increment display, no shift.
    
    writeFourBits(0x0C, LCD_COMMAND, 40, LOWER); //turn the display on, cursor off, blink off

}
    


 

/*
 * Clear the display.
 */
void clearLCD(){
    writeLCD(0x01, LCD_COMMAND, 1640);    
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y){
    //REMEMBER, THIS IS ALL HEXIDECIMAL
    //x: 0 -> top row, 1 -> bottom row
    //y: # of spaces over from the left (IE 0 is the leftmost, 1 is one space from the left, etc)
    unsigned char addr = 0x80 + (0x40 * x) + y; //see memory table. 0 -> 0x0<y>, 1 -> 0x4<y>, so to set the left digit in hex
    // we multiply it by 4, and shift left by four bits. y is indexed 0, 1, ..., 7, so we do NOT need to shift this.
    writeLCD(addr, LCD_COMMAND, 40); //RS must be zero for this command (see data sheet)).

}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD(){
    initLCD();
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 1000; i++) delayUs(1000);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 1000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for(i = 0; i < 1000; i++) delayUs(1000);
}

