#ifndef __LED_H
#define __LED_H

#include <stdint.h>
#include <string.h>

#include <stdlib.h>

typedef struct LED_cell{
	
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;

	uint8_t Data [16];
}LED_param;

void LED_init(LED_param* LED);
void setColor(LED_param* LED, uint16_t red, uint16_t green, uint16_t blue);
void TransmitData(uint8_t* Data);
void TransmitEnd (uint8_t nCells);
void TransmitStart (void);
void TransmitAll(LED_param LED[], uint8_t nCells);
void updateData(LED_param* LED);


#endif
