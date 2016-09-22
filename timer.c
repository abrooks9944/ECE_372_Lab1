/* 
 * File:   timer.c
 * Author: Alex Brooks
 *
 */

#include <xc.h>
#include <math.h>

void delayUs(int usec)
{
    TMR1 = 0;
    PR1 = calculatePR(usec); // .05 Seconds
    T1CONbits.TCKPS = 00;
    T1CONbits.TCS = 0;
    T1CONbits.TON = 1; // Start the timer  
    IPC1bits.T1IP = 7; //assign the interrupt priority. 
    while(IFS0bits.T1IF == 0); // Do nothing until the flag goes up
    IFS0bits.T1IF = 0; // Put the flag back down
    T1CONbits.TON = 0; // Turn off the timer    
}


int calculatePR(int usec) 
{
    floor((float)(usec * 10)) - 1; //prescalar of 1.
}


/* *****************************************************************************
 End of File
 */
