/*
 * DRV10987.c
 *
 *  Created on: 15 Aug 2019
 *      Author: Lenovo
 */

#include "DRV10987.h"

#define DRVI2Cadd 0x52

void statusRegRead( I2C_HandleTypeDef * hi2c, uint8_t * regBuf )
{
	int i;
	//uint8_t tempBuff[2]={0};
	for(i=0;i<8;i++){
		HAL_I2C_Mem_Read(hi2c,(DRVI2Cadd<<1),(i),1,&regBuf[2 * i]/*tempBuff*/,2,1000);
		//regBuf[2*i]=tempBuff[0];
		//regBuf[2*i+1]=tempBuff[1];
		HAL_Delay(5);
	}
}

static void waitReadyState(I2C_HandleTypeDef * hi2c){
	uint8_t tempBuff[2]={0};
	do{
		HAL_I2C_Mem_Read(hi2c,(DRVI2Cadd<<1),0x32,1,tempBuff,2,1000);
	}while((tempBuff[1]&1)==0);

}

//#define WORD16(v) ((v) & 0xFF), ((v) >> 8)
#define WORD16(v) ((v) >> 8), ((v) & 0xFF)

void configRegWrite(I2C_HandleTypeDef * hi2c, uint8_t * configVals )
{
	static uint8_t vals[] = {
		WORD16(0x8000),
		WORD16(0x0000),
		WORD16(0xC0DE),
		WORD16(0x0006),
		WORD16(0x0002),
	};
	uint8_t configReadBack[14] = {0};

	HAL_I2C_Mem_Write(hi2c, (DRVI2Cadd<<1),0x60,1,&vals[0],2,10);//Shit that the datasheet says to do on P20
	HAL_I2C_Mem_Write(hi2c, (DRVI2Cadd<<1),0x31,1,&vals[2],2,10);
	HAL_I2C_Mem_Write(hi2c, (DRVI2Cadd<<1),0x31,1,&vals[4],2,10);

	waitReadyState(hi2c);//Wait for EEPROM as P20

	int i;
	for (i=0;i<7;i++){
		HAL_I2C_Mem_Write(hi2c, (DRVI2Cadd<<1),(0x90+i),1,&configVals[2*i],2,10);
	}

	HAL_I2C_Mem_Write(hi2c, (DRVI2Cadd<<1),0x35,1,&vals[6],2,10);// Actually write shadow registers to the EEPROM

	waitReadyState(hi2c);//Wait...

	//Read back the values into configReadBack
	HAL_I2C_Mem_Write(hi2c, (DRVI2Cadd<<1),0x35,1,&vals[8],2,10);//Tell it to copy EPROM into shadow registers
	waitReadyState(hi2c);//Wait...

	for (i=0;i<7;i++){
			HAL_I2C_Mem_Read(hi2c, (DRVI2Cadd<<1),(0x90+i),1,&configReadBack[2*i],2,10);
			if ((configReadBack[ 2*i   ] != configVals[ 2*i   ])
		     || (configReadBack[(2*i)+1] != configVals[(2*i)+1]))
			{
				//error message
				//break;
			}
		}


	//Re-enable the motor
	HAL_I2C_Mem_Write(hi2c, (DRVI2Cadd<<1),0x60,1,&vals[2],2,10);



}


