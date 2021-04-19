#include <libopencm3/lm4f/uart.h>
#include <libopencm3/lm4f/systemcontrol.h>
#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/rcc.h>

enum {
	PLL_DIV_80MHZ 	= 5,
	PLL_DIV_57MHZ 	= 7,
	PLL_DIV_40MHZ 	= 10,
	PLL_DIV_20MHZ 	= 20,
	PLL_DIV_16MHZ 	= 25,
};

void SerialConfig(void);
void Serial1Config(void);
void BoardInit(void);
void SerialPrint(char *string);
void SerialPrintln(char *string);