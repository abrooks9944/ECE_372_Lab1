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
#include <xc.h>

#define INPUT 1
#define OUTPUT 0

#define ENABLED 1 
#define DISABLED 0

void initSwitch1(){
    TRISAbits.TRISA7 = INPUT; 
    CNPUAbits.CNPUA7 = ENABLED;
    
    CNCONAbits.ON = 1;//enable overall CN Interrupt CNCONx
    CNENAbits.CNIEA7 = 1;//Enable pin CN CNENx
    IFS1bits.CNAIF = 0;//put down the flag //IFSx IF THIS HAPPENS, INTERRUPT SERVICE ROUTINE WILL BE CALLED FOREVER. BAD.
    IPC8bits.CNIP = 7;//set interrupt priority //IPCx
    IEC1bits.CNAIE = 1;    //Enable Interrupt for D pins

}


/* *****************************************************************************
 End of File
 */
