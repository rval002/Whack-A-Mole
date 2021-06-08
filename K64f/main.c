/*****************************************************************************
 * Air Hockey Table Project
 *
 * Author: Oscar Carreon, Ricardo Valverde
 * --------------------------------------------------------------------------
 *           This Code is Created for UCR's EE128 Mini Project
 *                          Wack - A - Mole
 *
 *
 *************************************************************************** /

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "FX1.h"
#include "GI2C1.h"
#include "WAIT1.h"
#include "CI2C1.h"
#include "CsIO1.h"
#include "IO1.h"
#include "MCUC1.h"
#include "SM1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

#include <stdlib.h>


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */

unsigned char write[512];


// one sec delay
void software_delay(unsigned long delay){
	while(delay>0) delay --;
}
unsigned long Delay = 0x100000;



int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/************************* ADC PORTS *****************************************/
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; /*Enable Port A Clock Gate Control*/
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; /*Enable ADC Clock Gate Control*/
	ADC0_CFG1 = 0x0C; //16bits ADC; Bus Clock
	ADC0_SC1A = 0x1F; //Disable the module, ADCH = 11111

	/*****************************************************************************/

	/****************************** PORTB ****************************************/
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;//B
	PORTB_GPCLR = 0x060C0100;//Configure Port B Pins 0-7 for GPIO;
	GPIOB_PDDR = 0x0000060C;//Configure Port B Pins 0-7 for Output;

	// light up LED for testing
	//	GPIOB_PSOR = 0x04;
	//	software_delay(Delay+ (Delay * 4));
	//	GPIOB_PSOR = 0x08;
	//	GPIOB_PSOR = 0x200;
	//	GPIOB_PSOR = 0x400;

	/******************************************************************************/

	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/
	/* Write your code here */
	uint32_t delay;
	uint8_t ret, who;
	int8_t temp, data;
	int16_t accX, accY, accZ;
	int16_t magX, magY, magZ;

	// Score keeping
	int score = 1;
	// LED being light up
	int lightItUp = 0x00;

	// Photoresistor inputs values
	unsigned int trigger1 = 0;
	unsigned int trigger2 = 0;
	unsigned int trigger3 = 0;
	unsigned int trigger4 = 0;
	int triggerCheck = 0x00;
	int randNum=0;
	int win = 0;
	int  numCheck = 0;



	int len;
	LDD_TDeviceData *SM1_DeviceData;
	SM1_DeviceData = SM1_Init(NULL);


	FX1_Init();

	for(;;) {


		/*************************************************
		 * game logic psuedocode
		 *  rand() = lightItUp;
		 *
		 *  if(light  up > 0 $ < 5)
		 *
		 *    pinX = lightitup
		 *    check trigger
		 *    if only one trigger set triggercheck to led number
		 *
		 * 	 if triggercheck == lightitupt
		 * 	 score +=
		 * 	 if score > 0
		 * 	 send score to arduino
		 *
		 **************************************************/

		for(int i = 100; i > 0; i--){randNum = rand() % 4 +1;};

		if(randNum == 1){
			lightItUp = 0x04;
		}
		else if (randNum == 2){
			lightItUp = 0x08;
		}
		else if (randNum == 3){
			lightItUp = 0x200;
		}
		else if (randNum == 4){
			lightItUp = 0x400;
		}

		if(lightItUp == 0x04){
			GPIOB_PSOR = lightItUp;
			software_delay(Delay*0.5);

			trigger1 = ADC_read16bPH1();
			trigger1 = (trigger1 * 33) / 65535;
		}

		if(lightItUp == 0x08){
			GPIOB_PSOR = lightItUp;
			software_delay(Delay);
			trigger2 = ADC_read16bPH2();
			trigger2 = (trigger2 * 33) / 65535;
		}
		if(lightItUp == 0x0200){
			GPIOB_PSOR = lightItUp;
			software_delay(Delay);
			trigger3 = ADC_read16bPH3();
			trigger3 = (trigger3 * 33) / 65535;
		}
		if(lightItUp == 0x400){
			GPIOB_PSOR = lightItUp;
			software_delay(Delay);
			trigger4 = ADC_read16bPH4();
			trigger4 = (trigger4 * 33) / 65535;
		}

		if(trigger1 > 12){
			triggerCheck = 0x04;
		}
		if(trigger2 > 12){
			triggerCheck = 0x08;
		}
		if(trigger3 > 12){
			triggerCheck = 0x200;
		}
		if(trigger4 > 12){
			triggerCheck = 0x400;
		}

		if(triggerCheck == lightItUp){
			score = score + 1;
			if (score > 0) {
				win = 1 + win;
				// sending score to Arduino
				printf("%4d\n",score);
				len = sprintf(write, "%4d\n",score);
				SM1_SendBlock(SM1_DeviceData, &write, len);

				GPIOB_PCOR = 0xFFF;
				software_delay(Delay*0.5);
				GPIOB_PSOR = 0x060C;
				software_delay(Delay*0.5);
				GPIOB_PCOR = 0xFFF;
				software_delay(Delay*0.5);
				GPIOB_PSOR = 0x060C;
				software_delay(Delay*0.5);
				GPIOB_PSOR = 0xFFF;
				software_delay(Delay);
			}
		}
 /******************* Win LED sequence ***************************/

		if (win == 3) {


			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x400;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x04;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x08;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x200;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x400;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x04;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x08;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x200;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x400;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x04;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x08;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x200;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x400;
			software_delay(Delay*0.5);

			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x60C;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x60C;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x60C;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			GPIOB_PSOR = 0x60C;
			software_delay(Delay*0.5);
			GPIOB_PCOR = 0xFFF;
			software_delay(Delay*0.5);
			break;
		}
/**************************************************************************/

		software_delay(Delay*0.5);

// Reseting all values
		GPIOB_PCOR = 0xFFF;
		lightItUp = 0x00;
		randNum = 0;
		triggerCheck = 0x00;


		software_delay(Delay);


	}



	/* For example: for(;;) { } */

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
	/*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
#ifdef PEX_RTOS_START
	PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
#endif
	/*** End of RTOS startup code.  ***/
	/*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
	for(;;){}
	/*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/


int ADC_read16bPH1(void)// ADCO_PDO
{
	ADC0_SC1A = 0x00; //Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Until conversion complete
	return ADC0_RA;
}
int ADC_read16bPH2(void) // ADC0_DP1
{
	ADC0_SC1A = 0x01; //Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Until conversion complete
	return ADC0_RA;
}
int ADC_read16bPH3(void) //ADC_SE18
{
	ADC0_SC1A = 0x13; //Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Until conversion complete
	return ADC0_RA;
}
int ADC_read16bPH4(void) //ADC1_DP0
{
	ADC0_SC1A = 0x03; //Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Until conversion complete
	return ADC0_RA;
}


/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.4 [05.11]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
