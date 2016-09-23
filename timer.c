/* 
 * File:   timer.c
 * Author: Alex Brooks
 *
 */

#include <xc.h>
#include <math.h>
#define TRUE 1
#define FALSE 0


static char newTime[9]; //needs to be static to be returned.



void delayUs(int usec)
{
    TMR2 = 0;
    PR2 = calculatePR(usec); // .05 Seconds
    T2CONbits.TCKPS = 3; //prescalar of 8.
    T2CONbits.TCS = 0;
    T2CONbits.TON = 1; // Start the timer  
    IPC2bits.T2IP = 7; //assign the interrupt priority. 
    while(IFS0bits.T2IF == 0); // Do nothing until the flag goes up
    IFS0bits.T2IF = 0; // Put the flag back down
    T2CONbits.TON = 0; // Turn off the timer    
}


int calculatePR(int usec) 
{
    floor((float)(usec * 10))/8 - 1; //prescalar of 8.
}



char* updateTime(char* currentTime) //parse the character array & update the value.
{
    //all values are subtracted by 48, since the ASCII value of zero is 48.
    int msec = ((int)currentTime[6] - 48) * 10 + ((int)currentTime[7] - 48);
    int sec = ((int)currentTime[3] - 48) * 10 + ((int)currentTime[4] - 48);
    int minutes = ((int)currentTime[0] - 48) * 10 + ((int)currentTime[1] - 48);
    int msec_new = 0;//new values to be updated
    int sec_new = sec;
    int minutes_new = minutes;
    msec_new = (msec > 0 && ((msec + 1) % 100 != 0)) ? msec + 1 : 0;
    if(msec_new == 0 && msec != 0)//overflow, since msec reset itself.
        sec_new = ((sec + 1) % 60 != 0) ? sec + 1 : 0;
    if(msec_new == 0 && msec != 0 && sec_new == 0 && sec != 0) //double overflow!
        minutes_new = (minutes + 1 % 100 == 0) ? 0 : minutes + 1; //complete overflow
    
    newTime[0] = (((int)minutes_new) / 10) + 48;
    newTime[1] = (minutes_new  % 10) + 48;
    newTime[2] = ':';
    newTime[3] = (((int)sec_new) / 10) + 48;
    newTime[4] = (sec_new  % 10) + 48;
    newTime[5] = ':';
    newTime[6] = (((int)msec_new) / 10) + 48;
    newTime[7] = (msec_new  % 10) + 48;
    newTime[8] = '\0';
    
    return newTime;
}



char* clearTime(void)
{
    int i = 7;
    for(; i >= 0; i--)
    {
        newTime[i] = (i == 2 || i == 5) ? ':' : '0'; //newTime[2] & newTime[5] -> :, everything else -> 0.
    }
    return newTime;
}




/* *****************************************************************************
 End of File
 */
