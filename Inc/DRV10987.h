/*
 * DRV10987.h
 *
 *  Created on: 15 Aug 2019
 *      Author: Lenovo
 */

#ifndef DRV10987_H_
#define DRV10987_H_

#include "stm32f0xx_hal.h"

#include "stdint.h"

void statusRegRead( I2C_HandleTypeDef * hi2c, uint8_t * regBuf );
void configRegWrite(I2C_HandleTypeDef * hi2c, uint8_t * configVals);
#endif /* DRV10987_H_ */
