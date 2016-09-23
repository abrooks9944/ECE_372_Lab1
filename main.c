
/* 
 * File:   main.c
 * Author: Alex Brooks, Dallon Lines, Nathan Yokley
 * Description: lab 0 Part III.
 * Created on September 15, 2016
 */

/***************************************************************************
 *                           Part 1 <working>                              *
 ***************************************************************************/



/*
#include <xc.h>
#include <sys/attribs.h>
#include <proc/p32mx470f512l.h>
#include "timer.h"
#include "led.h"
#include "interrupt.h"
#include "config.h"
#include "switch.h"
#define OUTPUT 0
#define INPUT 1

typedef enum stateEnum { //all states are for debouncing; states handle LEDs.
    waitPress,
    waitRelease,
    debouncePress,
    debounceRelease     
} stateType;

volatile stateType state = waitPress;


void main() {
    SYSTEMConfigPerformance(10000000); //Important for stability
    enableInterrupts();
    initLEDs();
    preSetLEDs(); //clear LEDS except for the RUN_LED.
    
    initSwitch1();

    while (1) {
        switch (state) {
            case waitPress:
                break;
            case debouncePress:
                delayTimer1(); //debounce by .05 seconds
                state = waitRelease;
                break;
            case waitRelease:
                break;
            case debounceRelease:
                delayTimer1(); //debounce by .05 seconds
                toggleLEDs(); //flip the LEDs
                state = waitPress;                
                break;

        }
    }
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void) {
    IFS1bits.CNAIF = 0;
    int x;
    x = PORTA; // clearing the mismatch condition
   //debounce the button. 
    if (state == waitPress) {
        state = debouncePress;
    } 
    else if (state == waitRelease) {
        state = debounceRelease;
    }
}
 * 
 */


/***************************************************************************
 *                           Part 2 <to be tested>                         *
 ***************************************************************************/

/*
#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"


// ******************************************************************************************* //

int main(void)
{
    SYSTEMConfigPerformance(10000000);
    while(1)
    {
        testLCD();
    }
    
    return 0;
}

*/


/***************************************************************************
 *                           Part 3 <to be tested>                         *
 ***************************************************************************/



 
#include <xc.h>
#include <sys/attribs.h>
#include <proc/p32mx470f512l.h>
#include <string.h> 
#include "timer.h"
#include "led.h"
#include "interrupt.h"
#include "config.h"
#include "switch.h"
#define OUTPUT 0
#define INPUT 1

#define TORUN 1
#define TOPAUSE 0


typedef enum stateEnum { //all states are for debouncing; states handle LEDs.
    pause,
    run,
    clear,
    waitPress,
    waitRelease,
    debouncePress,
    debounceRelease     
} stateType;

volatile stateType state = pause;
volatile int direction = TORUN; //start in pause.

int main(void) {
    SYSTEMConfigPerformance(10000000); //Important for stability
    enableInterrupts();
    initLEDs();
    preSetLEDs(); //clear LEDS except for the RUN_LED.
    initSwitch1();
    char currentTime[] = "00:00:00";

    while (1) {
        switch (state) {
            case pause:
                break;
            case run:
                delayUs(1000); //delay 1 ms. every update, increment the time.
                strcpy(currentTime, updateTime(currentTime)); //add 1 msec
                printStringLCD(currentTime);   
                break;
            case clear:
                strcpy(currentTime, "00:00:00"); //clear the time
                printStringLCD(currentTime); //print the new time.
                state = pause;
                direction = TORUN;
                break;
            case debouncePress:
                delayUs(50000); //debounce by .05 seconds
                state = waitRelease;
                break;
            case waitRelease:
                break;
            case debounceRelease:
                delayUs(50000); //debounce by .05 seconds
                state = (direction == TORUN) ? run : pause;
                direction = (direction + 1) % 2; //flip the direction.
                break;

        }
    }
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt(void) {
    IFS0bits.T1IF = 0; //lower the flag 
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void) {
    IFS1bits.CNAIF = 0;
    //add second CN flag later.
    
    int x;
    x = PORTA; // clearing the mismatch condition
    
    if (state == pause) { //add && for appropriate flag.
        state = debouncePress;
    } 
    if (state == run) {
        state = debouncePress;
    }
    else if(IFS1bits.CNAIF == 1){ //reset. clear.
        state = clear;
        //add method for stopping the time updates.
    }
 
    
    
    else if (state == waitRelease) { //add && for appropriate flag.
        state = debounceRelease;
    }
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 