#include "basicfunctions.h"


int main(void) {
    char rx;

    //Initial configuration
    BoardInit();
    //Config and init Serial_0 e Serial_1
    SerialConfig();
    Serial1Config();
    
    while (1) {
        //Print via UART_0 (Serial_0)
        SerialPrintln("Hello World");
        rx = uart_recv_blocking(UART0);
        SerialPrint("Cool! You have entered: ");
        uart_send_blocking(UART0,rx);
    }

    return 0;
} 