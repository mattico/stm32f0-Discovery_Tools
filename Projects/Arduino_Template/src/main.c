// System includes
#include "stm32f0xx_conf.h"
#include "stm32f0_discovery.h"
#include "utilities.h"

// Arduino Includes
#include "gpio.h"

// Available arduino libraries are :
// 	- gpio.h
//	- spi.h
//	- serial.h

void setup() {

	// Insert Setup Code Here

}

void loop() {

	// Insert Loop Code Here

	// LED's aren't on arduino pins
	// so you have to use the functions from stm32f0_discovery.c
	STM_EVAL_LEDToggle(LED3);
	delay(500);
	STM_EVAL_LEDToggle(LED4);
	delay(500);

}


int main(void)
{
	DISCOVERY_Configuration();
	setup();
	while(1) {
		loop();
	}
}
