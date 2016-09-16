/* 
 * File:   led.c
 * Author: gvanhoy
 *
 * Created on August 27, 2015, 3:15 PM
 */

#include <xc.h>
#include <proc/p32mx470f512l.h>
#include "led.h"

#define RUN_TRIS TRISGbits.TRISG12
#define STOP_TRIS TRISGbits.TRISG14

#define RUN_LED LATGbits.LATG12
#define STOP_LED LATGbits.LATG14

#define LATCH 0
#define PORT 1

#define ON 0
#define OFF 1

void initLEDs(){    
    RUN_TRIS  = 0;
    STOP_TRIS = 0;       
}

void preSetLEDs(){ //turn on the running LED, turn off the other
    RUN_LED = ON;
    STOP_LED = OFF;
}


void toggleLEDs(){
    RUN_LED = (RUN_LED + 1) % 2; //1 -> 0, 0 -> 1
    STOP_LED = (STOP_LED + 1) % 2;
}
