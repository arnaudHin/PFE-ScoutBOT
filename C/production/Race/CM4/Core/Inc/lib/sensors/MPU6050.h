/*
 * MPU6050.h
 *
 *  Created on: 9 déc. 2021
 *      Author: martin
 */

#ifndef INC_LIB_SENSORS_MPU6050_H_
#define INC_LIB_SENSORS_MPU6050_H_

#include <lib/stm32mp1xx_i2c.h>
#include <lib/stm32mp1xx_uart.h>
#include "config.h"

#define MPU6050_ADDR 		0xD0

#define SMPLRT_DIV_REG 		0x19
#define GYRO_CONFIG_REG 	0x1B
#define ACCEL_CONFIG_REG 	0x1C
#define ACCEL_XOUT_H_REG 	0x3B
#define TEMP_OUT_H_REG 		0x41
#define GYRO_XOUT_H_REG 	0x43
#define PWR_MGMT_1_REG 		0x6B
#define WHO_AM_I_REG 		0x75


extern void MPU6050_Init (void);
extern void MPU6050_Read_Accel(void);
extern void MPU6050_Read_Gyro(void);

#endif /* INC_LIB_SENSORS_MPU6050_H_ */
