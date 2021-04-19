#include "basicfunctions.h"

/**
 * @brief Initial configuration
 * 
 */
void BoardInit(void) {
    //Set to GPIO aperture to AHB (mandatory for libopencm3 library)
    gpio_enable_ahb_aperture();
    //Set run clock
    //rcc_sysclk_config(OSCSRC_MOSC, XTAL_16M, PLL_DIV_80MHZ); /commenting this line leaves reset config (PIOSC)
}

/**
 * @brief Enable UART_0 (Serial_0). Pinned to PA0 (U0Rx) and PA1 (U0Tx)
 *   
 */
void SerialConfig(void) {
    //Enable the GPIO_A clock
    periph_clock_enable(RCC_GPIOA);
    //Set GPIO_A to function as UART_0 (alternate function)
    gpio_set_af(GPIOA,1,GPIO0|GPIO1);
    //To use the pin as a digital input or output (either GPIO or alternate function), the corresponding GPIODEN bit must be set
    //GPIO_DEN(GPIOA) |= (GPIO0|GPIO1);
    // Enable the UART_0 clock
    periph_clock_enable(RCC_UART0);
    // We need a brief delay before we can access UART config registers
    __asm__("nop"); __asm__("nop"); __asm__("nop");
    // Disable the UART while we mess with its settings
    uart_disable(UART0);
    // Configure the UART clock source as precision internal oscillator
    uart_clock_from_piosc(UART0);
    // Set communication parameters
    uart_set_baudrate(UART0, 115200);
    uart_set_databits(UART0, 8);
    uart_set_parity(UART0, UART_PARITY_NONE);
    uart_set_stopbits(UART0, 1);
    // Enable RTC and CTS lines
    //uart_set_flow_control(UART0, UART_FLOWCTL_RTS_CTS);
    // Now that we're done messing with the settings, enable the UART
    uart_enable(UART0);   
}

/**
 * @brief Enable UART_1 (Serial_1). Pinned to PB0 (U1Rx) and PB1 (U1Tx)
 * 
 */
void Serial1Config(void) {
    //Enable the GPIO_B clock
    periph_clock_enable(RCC_GPIOB);
    //Set GPIO_A to function as UART_1 (alternate function)
    gpio_set_af(GPIOB,1,GPIO0|GPIO1);
    //To use the pin as a digital input or output (either GPIO or alternate function), the corresponding GPIODEN bit must be set
    //GPIO_DEN(GPIOB) |= (GPIO0|GPIO1);
    // Enable the UART_1 clock
    periph_clock_enable(RCC_UART1);
    // We need a brief delay before we can access UART config registers
    __asm__("nop"); __asm__("nop"); __asm__("nop");
    // Disable the UART while we mess with its settings
    uart_disable(UART1);
    // Configure the UART clock source as precision internal oscillator
    uart_clock_from_piosc(UART1);
    // Set communication parameters
    uart_set_baudrate(UART1, 9600);
    uart_set_databits(UART1, 8);
    uart_set_parity(UART1, UART_PARITY_NONE);
    uart_set_stopbits(UART1, 1);
    // Enable RTC and CTS lines
    //uart_set_flow_control(UART1, UART_FLOWCTL_RTS_CTS);
    // Now that we're done messing with the settings, enable the UART
    uart_enable(UART1);   
}

/**
 * @brief Print using UART_0 (Serial_0)
 * 
 * @param string
 */
void SerialPrint(char *string) {
    //If there is something to be printed
    while (*string != '\0') {
        //Print a character (8 bits) using UART_0 (Serial_0)
        uart_send_blocking(UART0,*string);
        //next char
        string++;
    }
}

/**
 * @brief Print using UART_0 (Serial_0) + new line + carriage return 
 * 
 * @param string 
 */
void SerialPrintln(char *string) {
    SerialPrint(string);
    //end of line. new line
    SerialPrint("\n\r");
}