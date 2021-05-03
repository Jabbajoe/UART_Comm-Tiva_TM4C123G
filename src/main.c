#include "basicfunctions.h"

char sendbuffer[] = "Hello World!";

int main(void) {
    //Init board
    BoardInit();

    //Init and config Serial_1
    Serial1Config();

    //Loop
    while(1) {
        //Send message via UART_1
        SerialPrint(UART1,sendbuffer);

        //delay via software. what is the correlation between USER_DELAY and time (in second)? Is it stable?
        //SoftwareDelay(25000000);

        //delay in milliseconds via interrupt from SysTick
        HardwareDelay(5000);
    }

    return 0;
}

