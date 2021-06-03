
/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include <stdlib.h>

void software_delay(unsigned long delay){
	while(delay>0) delay --;
}
unsigned long Delay = 0x100000;

//static int i = 0;
unsigned char CNT_DIR = 0;
unsigned char ROT_DIR = 0;
unsigned int trigger1 = 0;
unsigned int trigger2= 0;
unsigned int trigger3 = 0;
unsigned int trigger4 = 0;
unsigned int lightItUp = 0x0;
unsigned char counter = 0x00;
unsigned char rotate = 0x10;
unsigned long voltagevalue =0x00;
unsigned long adcdata = 0x00;

unsigned int timer_val;
int randNum = 0;



int main(void)
{
//	while(1){
//randNum = rand() % 4 +1;
//	}
//	// clock timer
//	SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK; // FTM3 clock gating on
//	FTM3_MODE = 0x05; // Enable FTM3
//	FTM3_MOD = 0xFFFF; // Max counter value
//	// Set prescale factor and clock source (FTMx_SC[4:0])
//	FTM3_SC = 0x0F; // 0x0F => b01111 => system clock / 128
//
//	timer_val = FTM3_CNT; // Get current counter value
//



	//




	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; /*Enable Port A Clock Gate Control*/
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; /*Enable ADC Clock Gate Control*/


	ADC0_CFG1 = 0x0C; //16bits ADC; Bus Clock
	ADC0_SC1A = 0x1F; //Disable the module, ADCH = 11111

	//Configure Clock Gating for Ports:
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;//B
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;//C
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;//D

//	PORTB_GPCLR = 0xFFFF0100;//Configure Port B Pins 0-7 for GPIO;

	PORTB_GPCLR = 0x060C0100;//Configure Port B Pins 0-7 for GPIO;
	PORTC_GPCLR = 0xFFF0100;//Configure Port C Pins 0-5 and 7-8 for GPIO;

	PORTD_GPCLR = 0x000C0100;//Configure Port D Pin 1 for GPIO;


	GPIOD_PDDR = 0x00000000;//Configure Port D Pin 1 for Input;


//	GPIOB_PDDR = 0x000000FF;//Configure Port B Pins 0-7 for Output;
	GPIOB_PDDR = 0x0000060C;//Configure Port B Pins 0-7 for Output;
	GPIOC_PDDR = 0x000000DF;//Configure Port C Pins 0-5 and 7-8 for Output;


	GPIOC_PDOR = 0x01;//Initialize Port C to 0;
//	GPIOA_PDOR = 0x01;//Initialize Port B set 1 bit on
//	GPIOB_PSOR = 0x01;//Initialize Port B set 1 bit on
	GPIOB_PSOR = 0x04;
	software_delay(Delay+ (Delay * 4));
//	GPIOB_PSOR = 0x08;
//	GPIOB_PSOR = 0x200;
//	GPIOB_PSOR = 0x400;


	while (1) {


//		lightItUp = 0x01;
//		GPIOB_PSOR = lightItUp;

		GPIOB_PSOR = lightItUp;
		trigger1 = ADC_read16bPH1();
		trigger1 = (trigger1 * 33) / 65535;

		trigger2 = ADC_read16bPH2();
		trigger2 = (trigger2 * 33) / 65535;

		trigger3 = ADC_read16bPH3();
		trigger3 = (trigger3 * 33) / 65535;

		trigger4 = ADC_read16bPH4();
		trigger4 = (trigger4 * 33) / 65535;




		if(trigger1 > 15){
			lightItUp = 0x04;
		}
		if(trigger2 > 15){
			lightItUp = 0x08;
		}
		if(trigger3 > 15){
			lightItUp = 0x200;
		}
		if(trigger4 > 15){
			lightItUp = 0x400;
		}

		


//		for (int i = 0; i < 100000000; i++);
		GPIOB_PCOR = 0xFFF;
		lightItUp = 0x00;

	}
	return 0;
}

int ADC_read16bPH1(void)
{
	ADC0_SC1A = 0x00; //Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Until conversion complete
	return ADC0_RA;
}
int ADC_read16bPH2(void)
{
	ADC0_SC1A = 0x01; //Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Until conversion complete
	return ADC0_RA;
}
int ADC_read16bPH3(void)
{
	ADC0_SC1A = 0x13; //Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Until conversion complete
	return ADC0_RA;
}
int ADC_read16bPH4(void)
{
	ADC0_SC1A = 0x03; //Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Until conversion complete
	return ADC0_RA;
}

















//int ADC_read16b(void)
//{
//	ADC0_SC1A = 0x1A; //Write to SC1A to start conversion from ADC_0
//	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
//	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Until conversion complete
////	return ADC0_RA;
////}
//
//	adcdata = ADC0_RA;
//	voltagevalue = (adcdata*33)/65535;//conversion
//	if(voltagevalue > 1){
//		trigger = 1;
//	}
//	else{
//		trigger = 0;
//	}
//	return trigger;
//
//}


//void PORTA_IRQHandler(void)
//{
//
//
//}













