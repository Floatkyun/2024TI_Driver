/**
  ******************************************************************************
  * @file    AD9910.h
  * @author  LMH(2019.8.6) 2024.7.2 WJX 移植HAL库
  * @version V1.0
  * @date    06-August-2019 2024.7.2
  * @brief   AD9910 Driver.
  *          Function: 1.Single Tone.                            2.RAM Player. 
  *                    3.Digital Ramp Generater(Not Complicate). 4.Parallel Modulation(Not Complicate).
  *             
  * 1.  
  * 2. 
  * 3. 
  * 4. 
  * 5. 
  *
	引脚连线
	RST		PG2
	PF2		PG4
	PF0		PG6		
	IOUP(IO_UPDATE)	PG8
	DRO		PC7
	DRH		PC9
	
	PWR		PD11
	PF1		PD13
	OSK		PG3
	DRC		PG5
	SDIO	PG7
	SCK 	PC6
	CSB		PC8
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef __AD9910_H__
#define __AD9910_H__

#include "main.h"
#include "delay.h"

#define MHz 1000000
#define KHz 1000



//外设总线定义



//Port定义
#define AD9910_RST_Port 			GPIOG
#define AD9910_PF2_Port 			GPIOC
#define AD9910_PF0_Port 			GPIOA
#define AD9910_IOUP_Port			GPIOC
#define AD9910_DRO_Port 			GPIOD
#define AD9910_DRH_Port 			GPIOC

#define AD9910_PWR_Port 			GPIOC
#define AD9910_PF1_Port 			GPIOC
#define AD9910_OSK_Port 			GPIOG
#define AD9910_DRC_Port 			GPIOD
#define AD9910_SDIO_Port 			GPIOB
#define AD9910_SCK_Port 			GPIOB
#define AD9910_CSB_Port 			GPIOC


//Pin定义
#define AD9910_RST_Pin 			GPIO_PIN_7
#define AD9910_PF2_Pin 			GPIO_PIN_9
#define AD9910_PF0_Pin 			GPIO_PIN_8
#define AD9910_IOUP_Pin			GPIO_PIN_10
#define AD9910_DRO_Pin 			GPIO_PIN_7
#define AD9910_DRH_Pin 			GPIO_PIN_7

#define AD9910_PWR_Pin 			GPIO_PIN_12
#define AD9910_PF1_Pin 			GPIO_PIN_11
#define AD9910_OSK_Pin 			GPIO_PIN_15
#define AD9910_DRC_Pin 			GPIO_PIN_6
#define AD9910_SDIO_Pin 		GPIO_PIN_5
#define AD9910_SCK_Pin 			GPIO_PIN_3
#define AD9910_CSB_Pin 			GPIO_PIN_6


//引脚输出
#define AD9910_RST_0()		HAL_GPIO_WritePin(AD9910_RST_Port, AD9910_RST_Pin, GPIO_PIN_RESET);
#define AD9910_RST_1()		HAL_GPIO_WritePin(AD9910_RST_Port, AD9910_RST_Pin, GPIO_PIN_SET);

#define AD9910_PF2_0()		HAL_GPIO_WritePin(AD9910_PF2_Port, AD9910_PF2_Pin, GPIO_PIN_RESET);
#define AD9910_PF2_1()		HAL_GPIO_WritePin(AD9910_PF2_Port, AD9910_PF2_Pin, GPIO_PIN_SET);

#define AD9910_PF0_0()		HAL_GPIO_WritePin(AD9910_PF0_Port, AD9910_PF0_Pin, GPIO_PIN_RESET);
#define AD9910_PF0_1()		HAL_GPIO_WritePin(AD9910_PF0_Port, AD9910_PF0_Pin, GPIO_PIN_SET);

#define AD9910_IO_UPDATE_0()		HAL_GPIO_WritePin(AD9910_IOUP_Port, AD9910_IOUP_Pin, GPIO_PIN_RESET);
#define AD9910_IO_UPDATE_1()		HAL_GPIO_WritePin(AD9910_IOUP_Port, AD9910_IOUP_Pin, GPIO_PIN_SET);

#define AD9910_DRO_0()		HAL_GPIO_WritePin(AD9910_DRO_Port, AD9910_DRO_Pin, GPIO_PIN_RESET);
#define AD9910_DRO_1()		HAL_GPIO_WritePin(AD9910_DRO_Port, AD9910_DRO_Pin, GPIO_PIN_SET);

#define AD9910_DRH_0()		HAL_GPIO_WritePin(AD9910_DRH_Port, AD9910_DRH_Pin, GPIO_PIN_RESET);
#define AD9910_DRH_1()		HAL_GPIO_WritePin(AD9910_DRH_Port, AD9910_DRH_Pin, GPIO_PIN_SET);


#define AD9910_PWR_0()		HAL_GPIO_WritePin(AD9910_PWR_Port, AD9910_PWR_Pin, GPIO_PIN_RESET);
#define AD9910_PWR_1()		HAL_GPIO_WritePin(AD9910_PWR_Port, AD9910_PWR_Pin, GPIO_PIN_SET);

#define AD9910_PF1_0()		HAL_GPIO_WritePin(AD9910_PF1_Port, AD9910_PF1_Pin, GPIO_PIN_RESET);
#define AD9910_PF1_1()		HAL_GPIO_WritePin(AD9910_PF1_Port, AD9910_PF1_Pin, GPIO_PIN_SET);

#define AD9910_OSK_0()		HAL_GPIO_WritePin(AD9910_OSK_Port, AD9910_OSK_Pin, GPIO_PIN_RESET);
#define AD9910_OSK_1()		HAL_GPIO_WritePin(AD9910_OSK_Port, AD9910_OSK_Pin, GPIO_PIN_SET);

#define AD9910_DRC_0()		HAL_GPIO_WritePin(AD9910_DRC_Port, AD9910_DRC_Pin, GPIO_PIN_RESET);
#define AD9910_DRC_1()		HAL_GPIO_WritePin(AD9910_DRC_Port, AD9910_DRC_Pin, GPIO_PIN_SET);

#define AD9910_SDIO_0()		HAL_GPIO_WritePin(AD9910_SDIO_Port, AD9910_SDIO_Pin, GPIO_PIN_RESET);
#define AD9910_SDIO_1()		HAL_GPIO_WritePin(AD9910_SDIO_Port, AD9910_SDIO_Pin, GPIO_PIN_SET);

#define AD9910_SCK_0()		HAL_GPIO_WritePin(AD9910_SCK_Port, AD9910_SCK_Pin, GPIO_PIN_RESET);
#define AD9910_SCK_1()		HAL_GPIO_WritePin(AD9910_SCK_Port, AD9910_SCK_Pin, GPIO_PIN_SET);

#define AD9910_CSB_0()		HAL_GPIO_WritePin(AD9910_CSB_Port, AD9910_CSB_Pin, GPIO_PIN_RESET);
#define AD9910_CSB_1()		HAL_GPIO_WritePin(AD9910_CSB_Port, AD9910_CSB_Pin, GPIO_PIN_SET);


#define AD9910_RESET()		do \
							{ \
								AD9910_RST_1(); \
								delay_us(1); \
								AD9910_RST_0(); \
							}while(0)
								
#define AD9910_UPDATE()		do \
							{ \
								AD9910_IO_UPDATE_1(); \
								AD9910_IO_UPDATE_0(); \
							}while(0)
							
typedef enum
{
	OFF = 0,
	ON,
}AD9910_Mod_State;

typedef enum
{
	Frequency = 0,
	Phase,
	Amplitude,
	Polar,
}Mod_Destination;


typedef enum
{
	Bypass = 0,
	Burst_0_1,
	Burst_0_2,
	Burst_0_3,
	Burst_0_4,
	Burst_0_5,
	Burst_0_6,
	Burst_0_7,
	Continuous_0_1,
	Continuous_0_2,
	Continuous_0_3,
	Continuous_0_4,
	Continuous_0_5,
	Continuous_0_6,
	Continuous_0_7,
}RAM_Ramp_Mod;

typedef enum
{
	Direct_Switch = 0,
	Ramp_UP,
	Bi_Ramp,
	Contiuous_Bi_Ramp,
	Recircircle,
}RAM_Opt_Mod;

typedef enum
{
	Dwell_Both = 0,
	Dwell_High,
	Dwell_Low,
	No_Dwell,
	
}DRG_Dwell_Mod;

typedef struct
{
	//控制寄存器相关控制字
	AD9910_Mod_State State;
	Mod_Destination Destin;
	RAM_Ramp_Mod Ramp_Mod;
	
	//Profile相关控制字
	RAM_Opt_Mod Operation_Mod;
	AD9910_Mod_State Zero_Crossing;
	AD9910_Mod_State No_Dwell_High;
	
	uint16_t Start_Address;
	uint16_t End_Address;
	float Step_Time;	//单位ns
	
	uint8_t Profile_ptr;
	
}RAM_Mod_Type;

typedef struct
{
	AD9910_Mod_State State;
	Mod_Destination Destin;
	DRG_Dwell_Mod Dwell_Mod;
	
}DRG_Mod_Type;

typedef struct
{
	AD9910_Mod_State State;
	AD9910_Mod_State AutoFunc;
	AD9910_Mod_State ExtFunc;
	
}OSK_Mod_Type;
//

typedef struct
{
	float system_clock;
	float pll_divider_ratio;
	float _2_32_divide_system_clock_and_pll_divider_ratio;
	
}AD9910TypeDef;	



extern AD9910TypeDef Extern_AD9910_Data;
extern RAM_Mod_Type RAM_Mod_Data;
extern DRG_Mod_Type DRG_Mod_Data;
extern OSK_Mod_Type OSK_Mod_Data;
extern uint8_t Profile_Reg[8];

//SPI接口封装/底层封装
void AD9910WriteByte(uint8_t Data);
void AD9910WtiteDatas(uint8_t Address, uint8_t* Data, uint16_t Length);
void AD9910WriteProfileReg(uint16_t Order, uint8_t* Data);
void AD9910_IO_Init(void);

//应用函数
void AD9910_Init(void);	
void AD9910SetPLLDividerRatio(uint8_t N);			
void ProfileSelect(uint8_t Num);
void AD9910_ModType_Cfig(RAM_Mod_Type RAM_Mod_Data, DRG_Mod_Type DRG_Mod_Data, OSK_Mod_Type OSK_Mod_Data);
void AD9910_Amp_Pha_Freq_Set(float amplitude, float phase, float frequency, uint16_t Profile_NUM);
uint8_t AD9910_RAM_ProfileCfig(RAM_Mod_Type RAM_Mod_Data);
uint8_t AD9910_RAM_Load(RAM_Mod_Type RAM_Mod_Data, uint8_t* RAMData);


void RamPlayer_test(float time);


#endif


