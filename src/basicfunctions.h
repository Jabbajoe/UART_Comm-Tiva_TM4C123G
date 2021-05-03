#include <libopencm3/lm4f/uart.h>
#include <libopencm3/lm4f/systemcontrol.h>
#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/lm4f/nvic.h>

/* This is how the RGB LED is connected on the stellaris launchpad */
//#define RGB_PORT	GPIOF
enum {
	LED_R	= GPIO1,
	LED_G	= GPIO3,
	LED_B	= GPIO2,
};

/* This is how the user switches are connected to GPIOF */
enum {
	USR_SW1	= GPIO4,
	USR_SW2	= GPIO0,
};

/* The PLL divisors */
enum {
	NO_PLL          = 0,
	PLL_DIV_80MHZ 	= 5,
	PLL_DIV_57MHZ 	= 7,
	PLL_DIV_40MHZ 	= 10,
	PLL_DIV_20MHZ 	= 20,
	PLL_DIV_16MHZ 	= 25,
};

// Function prototypes
void LEDsConfig(void);
void SerialConfig(void);
void Serial1Config(void);
void Serial2Config(void);
void BoardInit(void);
void SerialPrint(uint32_t uart, char *string);
void SerialPrintln(uint32_t uart, char *string);
void SoftwareDelay(unsigned long USER_DELAY);
void HardwareDelay(uint32_t ms);