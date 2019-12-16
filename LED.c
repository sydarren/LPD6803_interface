#include "igs_app.h" // include function is used only to interface into the LPC175x chip


/*
* Description: 
* Custom Interface code for LPD6803 LED strips
* LED Data array holds the (1,0) data to be pulsed out with [15] being the MSB
* Red, Green, and Blue variables alters its corresponding color (5 bit value)
*/
typedef struct LED_cell{
	
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;

	uint8_t Data [16];
}LED_param;


void updateData(LED_param* LED);

/*
* Constructor function
* Used to initiate the individual cell 
* @param: LED_param * is the address of an individual LED cell
*/
void LED_init(LED_param* LED)
{
	int16_t i;
	
	LED-> Red = 0;
	LED-> Blue = 0;
	LED-> Green=0;
	
	updateData(LED);
}

/*pass the data array
* Function invokes the GPIO pins of the MCU to pulse out the corresponding data 
* and clock pulses necessary.
* @param: Passes in the entire Data array to be cycled and printed out
*/
void TransmitData(uint8_t* Data)
{
	int16_t i;
	for(i=15; i>=0; i--)
	{
		GPIOSetValue(LPC_GPIO0, 22,  Data[i]);  // SDO 
		GPIOSetValue(LPC_GPIO0, 11,  GPIO_HIGH); // SCLK = 1
		GPIOSetValue(LPC_GPIO0, 11,  GPIO_LOW); // SCLK = 0 
	}
}

/*
* This function invokes the GPIO pins to transmit the start signal
* for the LED strip to indicate that it's start
*/
void TransmitStart (void)
{
	int16_t i;
	
	GPIOSetValue(LPC_GPIO0, 11,  GPIO_LOW); // SCLK = 0 
	GPIOSetValue(LPC_GPIO0, 22,  GPIO_LOW); // SDO = 0 (start is all 0) 
	
	for(i=31; i>=0; i--)
	{
		// GPIO ()
		GPIOSetValue(LPC_GPIO0, 11,  GPIO_HIGH); // SCLK = 1
		GPIOSetValue(LPC_GPIO0, 11,  GPIO_LOW); // SCLK = 0 
	}
	
}

/*
* This function invokes the GPIO pins to transmit the end signal
* for the LED strip to indicate that it's end
* @param: Pass the number of LED cells (1 cell == 3 LEDs) used 
*/
void TransmitEnd (uint8_t nCells) // number of LED cells
{
	int16_t i;
	
	GPIOSetValue(LPC_GPIO0, 11,  GPIO_LOW); // SCLK = 0 
	
	GPIOSetValue(LPC_GPIO0, 22,  GPIO_LOW); // SDO = 0 (end is all 0) 
	for(i=0; i<nCells; i++)
	{
		// GPIO ()
		GPIOSetValue(LPC_GPIO0, 11,  GPIO_HIGH); // SCLK = 1
		GPIOSetValue(LPC_GPIO0, 11,  GPIO_LOW); // SCLK = 0 
	}
	
}

/*
*	Function refreshes the data in the LED data cell array. 
*	@param: Pass the individual LED cell object and translates whatever
* value found in Red, Green, Blue variables into corresponding Serial data
*/
void updateData(LED_param* LED)
{
	uint8_t i, R, G, B;
	
	R = LED->Red;
	G = LED->Green;
	B = LED->Blue;
	
	// set the MSB to be 1 as stated in protocol
	LED->Data[15] =1;
	
	for (i=0; i<5; i++)
	{
		// set green data
		if(G & (0x01 <<(4-i)))
			LED->Data[4-i] = 1;
		else
			LED->Data[4-i] = 0;
		
		//set red data
		if(R & (0x01 <<(4-i)))
			LED->Data[9-i] = 1;
		else
			LED->Data[9-i] = 0;
		
		//set blue data
		if(B & (0x01 <<(4-i)))
			LED->Data[14-i] = 1;
		else
			LED->Data[14-i] = 0;
	}
}

/*
* Combines all tramsmission functions into one
*	Pass the entire LED cell array and the number of Cells
*/
void TransmitAll(LED_param LED[], uint8_t nCells)
{
	uint8_t i = 0;
	
	TransmitStart();
	
	for(i=0; i<nCells; i++)
	{
		updateData(&LED[i]);
		TransmitData(LED[i].Data);
	}
		
	TransmitEnd(nCells);
}

/* 
*	Edit all the colors passing an integer value of 0-31 and the LED cell to be changed
*	Updates the data array ofthe corresponding LED cell.
* @param: Pass in the individual LED cell, and the individual RGB intensity values chosen. 
* Function updateData is called to update the corresponding Data array. 
*/
void setColor(LED_param* LED, uint16_t red, uint16_t green, uint16_t blue)
{
	LED->Blue = blue;
	LED->Green = green;
	LED->Red = red;
	
	updateData(LED);
}

/*
* Updates the ENTIRE data array of the passed LED cell
*	@param LED_param the single cell typedef 
*		Color code - 5 bit color data
*/
void setData(LED_param* LED, uint16_t ColorCode)
{
		uint8_t i;
	for(i=0; i<15; i++)
	{
		if(ColorCode & (0x01 <<(14-i)))
			LED->Data[14-i] = 1;
		else 
			LED->Data[14-i] = 0;
	}
	
	LED->Data[15] = 1;
}
