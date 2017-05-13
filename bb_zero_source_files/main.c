#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dac.h"
#include "misc.h"
#include "sound.h"
#include "inits.h"
#include "IRQHandlers.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_i2c.h"
#include "tm_stm32f4_mpu6050.h"
#include "defines.h"
#include "attributes.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32_ahrs_imu.h"

int16_t Acc,Gyr;
float Acc_Result, Gyr_Result;
float Acc_Offset=750.0, Gyr_Offset=0.0;
float angle=0;
float motor;
float Gyr_Result2;
float ax,gx,ay,gy,az,gz;
TM_MPU6050_t MPU6050_Data0;

float target_position = -2.0;
float current_position;
float error,last_error=0.0;
float integral=0.0;
float derivative=0.0;


float kp=18;
float ki=0.7;
float kd=1;

void PID(float angle)
{

	current_position = angle;

	error = target_position - current_position;

	integral = integral + error;

	derivative = error - last_error;

	motor = (kp*error) + (ki*integral) + (kd*derivative);

	if(motor>99) motor = 99;
	if(motor<-99) motor = -99;

	last_error = error;
}

TM_AHRSIMU_t IMUstruct;
int main(void)
{




	    uint8_t sensor1 = 0;
	    /* Initialize system */
	    SystemInit();
	    TM_DELAY_Init();
	    TM_AHRSIMU_Init(&IMUstruct, 0.1 , 100, 67);
	    initAll();


	    /* Initialize MPU6050 sensor 0, address = 0xD0, AD0 pin on sensor is low */
	    if (TM_MPU6050_Init(&MPU6050_Data0, TM_MPU6050_Device_0, TM_MPU6050_Accelerometer_2G, TM_MPU6050_Gyroscope_2000s) == TM_MPU6050_Result_Ok)
	    {
	        /* Sensor 1 OK */
	        sensor1 = 1;
	    }


	    float x=0.98;
	    int i =0;
	    while (1)
	    {
	    	 /* Every 10ms */
	    	if (TM_DELAY_Time() >= 10) {//
	    	    /* Reset time */
	    		TM_DELAY_SetTime(0);
	    		i++;
	            /* If sensor 1 is connected */
	            if (sensor1)
	            {
	                TM_MPU6050_ReadAll(&MPU6050_Data0);
	            }
	            gx = (MPU6050_Data0.Gyroscope_X * 2000.0f) / 32768.0f;
	            gy = (MPU6050_Data0.Gyroscope_Y * 2000.0f) / 32768.0f;
	            gz = (MPU6050_Data0.Gyroscope_Z * 2000.0f) / 32768.0f;

				ax = (MPU6050_Data0.Accelerometer_X * 2.0f) / 32768.0f;
				ay = (MPU6050_Data0.Accelerometer_Y * 2.0f) / 32768.0f;
				az = (MPU6050_Data0.Accelerometer_Z * 2.0f) / 32768.0f;

	            TM_AHRSIMU_UpdateIMU(&IMUstruct, AHRSIMU_DEG2RAD(gx), AHRSIMU_DEG2RAD(gy), AHRSIMU_DEG2RAD(gz), ax, ay, az);

	            PID(IMUstruct.Pitch);

	            if(motor < 0)
	            {
	            	GPIO_SetBits(GPIOB,GPIO_Pin_13);//STBY

	            	GPIO_SetBits(GPIOB,GPIO_Pin_12);//AIN1
					GPIO_ResetBits(GPIOB,GPIO_Pin_11);//AIN2

					GPIO_SetBits(GPIOB,GPIO_Pin_4);//BIN2
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);//BIN1

					TIM4->CCR1=-motor;
					TIM4->CCR2=-motor;
	            }
	            else
	            {
	            	GPIO_SetBits(GPIOB,GPIO_Pin_13);//STBY

					GPIO_SetBits(GPIOB,GPIO_Pin_11);//AIN1
					GPIO_ResetBits(GPIOB,GPIO_Pin_12);//AIN2

					GPIO_SetBits(GPIOB,GPIO_Pin_5);//BIN2
					GPIO_ResetBits(GPIOB,GPIO_Pin_4);//BIN1

					TIM4->CCR1=motor;
					TIM4->CCR2=motor;
	            }


	    }
	    }
}
