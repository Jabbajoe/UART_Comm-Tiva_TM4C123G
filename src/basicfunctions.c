#include "basicfunctions.h"

// Global volatile variables that can be modified by Interrupt Service Routines
volatile uint32_t millisec = 0;


/*********************************************************************
 * 
 *      PRIVATE FUNCTIONS 
 * 
 *********************************************************************/

#define FLASH_DELAY 80000
static void internalsoftwaredelay(void) {
	int i;
	for (i = 0; i < FLASH_DELAY; i++)
		__asm__("nop");
}

/**
 * @brief SysTick timer Initialization
 * 
 */ 
static void SystickInit(void) {
    // Clear the Enable interrupt bit in 'STCTRL' to disable SysTick interrupt during initialization
    systick_interrupt_disable();

    // Clear the Enable bit in 'STCTRL' to disable SysTick counter during initialization
    systick_counter_disable();

    // Set reload to 1 millisecond (1 ms = 0xE7F (15999) ticks) 
    // 1/(clock frequency) = time of 1 tick (@16 MHz -> 62.5 ns)
    // Reload value [ticks] = (Time delay [ms] / time of 1 tick [= 0.0000625 ms]) - 1   
    //
    // @(PIOSC/4): 1 millisecond = 0xF9F (3999) ticks
    systick_set_reload(0xF9F);

    // Clear the counter
    systick_clear();

    // Select systick clock source
    //systick_set_clocksource(STK_CSR_CLKSOURCE_AHB); // Set system clock as source (rcc register reset config: PIOSC as system clock) (TM4C123GXL board can use 16 MHz XTAL + PLL)
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8); //// Set PIOSC divided by 4 as clock source 

    // Enable systick interrupt (Since SysTick is an internal interrupt, the COUNT flag is cleared automatically when the interrupt occurs)
    systick_interrupt_enable();

    // Enable the counter
    systick_counter_enable();
}

/**
 * @brief Interrupt Service Routine responsible for incrementing millis at each 1 millisecond
 * 
 */
void sys_tick_handler(void) {
    millisec++; // What happens when millis reaches maximum value (usigned 32 bit value)?
}



/*********************************************************************
 * 
 *      PUBLIC FUNCTIONS 
 * 
 *********************************************************************/


/**
 * @brief Initial configuration
 * 
 */
void BoardInit(void) {
// Set to GPIO aperture to AHB (mandatory for libopencm3 library)
    gpio_enable_ahb_aperture();

// Set run clock 
//
//** using external XTAL (in TM4C123GXL board -> 16 MHz) **
// When flashing using PlatformIO + OpenOCD, if this line is uncommented, returns a FAILURE
//          Error: memory read failed: 0x7
//          Error: memory write failed: 0x7
//          in procedure 'program'
//          *** [upload] Error 1
//
// using Energia IDE gives no error
    //rcc_sysclk_config(OSCSRC_MOSC, XTAL_16M, PLL_DIV_80MHZ); //commenting this line leaves reset config (PIOSC only as oscillator source)
//
//** using PIOSC **
    rcc_sysclk_config(OSCSRC_PIOSC, XTAL_16M, 0);



// Initialize Systick
    SystickInit();
}


/**
 * @brief Configure LEDs. In Tiva TM4C123G:
 *              LEDs_PORT -->  GPIO_F
 *              LED_Red	  -->  GPIO_1
 *              LED_Blue  -->  GPIO_2
 *              LED_Green -->  GPIO_3
 * 
 */
void LEDsConfig(void) {
//Enable the GPIO_F clock
    periph_clock_enable(RCC_GPIOF);

//Configure LED pins
    const uint32_t outpins = (LED_R | LED_G | LED_B);
    gpio_mode_setup(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, outpins);
	gpio_set_output_config(GPIOF, GPIO_OTYPE_PP, GPIO_DRIVE_2MA, outpins);
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
// Set flow control (RTC and CTS lines)
    uart_set_flow_control(UART0, UART_FLOWCTL_NONE);

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

//Set GPIO_B to function as UART_1 (alternate function)
    gpio_set_af(GPIOB, 1, GPIO1|GPIO0);

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
    uart_set_baudrate(UART1, 115200);
    uart_set_databits(UART1, 8);
    uart_set_parity(UART1, UART_PARITY_NONE);
    uart_set_stopbits(UART1, 1);
// Set flow control (RTC and CTS lines)
    //uart_set_flow_control(UART0, UART_FLOWCTL_NONE);
    
// Now that we're done messing with the settings, enable the UART
    uart_enable(UART1);   
}

/**
 * @brief Enable UART_2 (Serial_2). Pinned to PD6 (U2Rx) and PD7 (U2Tx)
 * 
 */
void Serial2Config(void) {
    //Enable the GPIO_D clock
    periph_clock_enable(RCC_GPIOD);
    //Set GPIO_D to function as UART_2 (alternate function)
    gpio_set_af(GPIOD,1,GPIO6|GPIO7);
    //To use the pin as a digital input or output (either GPIO or alternate function), the corresponding GPIODEN bit must be set
    //GPIO_DEN(GPIOD) |= (GPIO6|GPIO7);
    // Enable the UART_2 clock
    periph_clock_enable(RCC_UART2);
    // We need a brief delay before we can access UART config registers
    __asm__("nop"); __asm__("nop"); __asm__("nop");
    // Disable the UART while we mess with its settings
    uart_disable(UART2);
    // Configure the UART clock source as precision internal oscillator
    uart_clock_from_piosc(UART2);
    // Set communication parameters
    uart_set_baudrate(UART2, 115200);
    uart_set_databits(UART2, 8);
    uart_set_parity(UART2, UART_PARITY_NONE);
    uart_set_stopbits(UART2, 1);
    // Now that we're done messing with the settings, enable the UART
    uart_enable(UART2);   
}

/**
 * @brief 
 * 
 * @param USER_DELAY 
 */
void SoftwareDelay(unsigned long USER_DELAY)
{
	unsigned long i;
	for (i = 0; i < USER_DELAY; i++)
		__asm__("nop");
}

/**
 * @brief Delay using systick timer interrupt @16 MHz
 *  millis is incremented each 1 millisecond by the Interrupt Service Routine
 * 
 * @param ms Time of delay, in miliseconds
 */
void HardwareDelay(uint32_t ms) {
    //uint32_t temp = millisec;
    //while ( (millisec - temp) < ms) {
        // do nothing
    //}

    millisec = 0;
    while (millisec < ms) {
        // do nothing
    }
}

/**
 * @brief Print using UART (Serial)
 * 
 * @param string
 */
void SerialPrint(uint32_t uart, char string[]) {

    int i = -1;
    
    //Print string (including '\0')
    do {
        // Next char
        i++;

        //Print a character (8 bits) using UART
        uart_send_blocking(uart, string[i]);

        //delay necessário para manter o sistema estável (pq? tem a ver com o baud rate?)
        internalsoftwaredelay();

    } while (string[i] != '\0' && i < 20);

    if (i == 20) {
        uart_send_blocking(uart, '\0');
    }
}

/**
 * @brief Print using UART + new line + carriage return 
 * 
 * @param string 
 */
void SerialPrintln(uint32_t uart, char *string) {
    SerialPrint(uart, string);
    //end of line. new line
    SerialPrint(uart, "\n\r");
}