
/* 
 * File:   main.c
 * Author: Alex Brooks, Dallon Lines, Nathan Yokley
 * Description: lab 0 Part III.
 * Created on September 15, 2016
 */

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

typedef enum stateEnum {
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
    IFS1bits.CNDIF = 0;
    int x;
    x = PORTA; // clearing the mismatch condition

    if (state == waitPress) {
        state = debouncePress;
    } 
    else if (state == waitRelease) {
        state = debounceRelease;
    }
}