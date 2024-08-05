/*---------------------------------------------------------
modify author： yusnows
file：AD9959.h
version： v0.0
Date；2017.8.7
state： paseed
----------------------------------------------------------*/

#ifndef _AD9959_H_
#define _AD9959_H_
#include "main.h"
#include <math.h>
#include "arm_math.h"

#ifndef kHz
#define kHz 1000
#endif

#ifndef MHz
#define MHz 1000*kHz
#endif

#define SYSCLK  500*MHz

//AD9959寄存器地址定义
#define CSR_ADDR    0x00   //CSR 通道选择寄存器
#define FR1_ADDR    0x01   //FR1 功能寄存器1
#define FR2_ADDR    0x02   //FR2 功能寄存器2
#define CFR_ADDR    0x03   //CFR 通道功能寄存器
#define CFTW0_ADDR  0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADDR  0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADDR    0x06   //ACR 幅度控制寄存器
#define LSRR_ADDR   0x07   //LSR 通道线性扫描寄存器
#define RDW_ADDR    0x08   //RDW 通道线性向上扫描寄存器
#define FDW_ADDR    0x09   //FDW 通道线性向下扫描寄存器

#define CW1         0x0A    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW2         0x0B    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW3         0x0C    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW4         0x0D    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW5         0x0E    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW6         0x0F    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW7         0x10    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW8         0x11    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW9         0x12    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW10        0x13    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW11        0x14    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW12        0x15    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW13        0x16    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW14        0x17    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]
#define CW15        0x18    //Frequency tuning word[31:0] or phase word[31:18] or amplitude word[31:22]

//AD9959线性扫频寄存器地址定义
#define FREQS0_ADDR     0x04
#define FREQE0_ADDR     0x0A
#define PHASES0_ADDR    0x05
#define PAHSEE0_ADDR    0x0A
#define AMPS0_ADDR      0x06
#define AMPE0_ADDR      0x0A
#define RSRR_ADDR       0x07
#define FSRR_ADDR       0x07


//AD9959 AFP bits id in CFR[23:22]
#define FREQSWEEP       ((uint32_t)0x10<<22)
#define AMPSWEEP        ((uint32_t)0x01<<22)
#define PHASESWEEP      ((uint32_t)0x11<<22)

//ADF9959 Modulation Level for linear sweep, in FR1[9:8]
#define LSMODU          ((uint32_t)0x00<<8)

#define AD9959_SDIO0_GPIO 	GPIOC
#define AD9959_SDIO1_GPIO 	GPIOD
#define AD9959_SDIO2_GPIO 	GPIOC
#define AD9959_SDIO3_GPIO 	GPIOC
#define AD9959_PS0_GPIO 		GPIOG
#define AD9959_PS1_GPIO 		GPIOC
#define AD9959_PS2_GPIO 		GPIOA
#define AD9959_PS3_GPIO 		GPIOD
#define AD9959_SCLK_GPIO 		GPIOD
#define AD9959_CS_GPIO 			GPIOG
#define AD9959_UPDATE_GPIO 	GPIOC
#define AD9959_RESET_GPIO 	GPIOC
#define AD9959_PDC_GPIO 		GPIOC

#define AD9959_SDIO0_Pin 	GPIO_PIN_10
#define AD9959_SDIO1_Pin 	GPIO_PIN_7
#define AD9959_SDIO2_Pin 	GPIO_PIN_7
#define AD9959_SDIO3_Pin 	GPIO_PIN_6
#define AD9959_PS0_Pin 		GPIO_PIN_7
#define AD9959_PS1_Pin 		GPIO_PIN_9
#define AD9959_PS2_Pin 		GPIO_PIN_8
#define AD9959_PS3_Pin 		GPIO_PIN_2
#define AD9959_SCLK_Pin 		GPIO_PIN_6
#define AD9959_CS_Pin 			GPIO_PIN_15
#define AD9959_UPDATE_Pin 	GPIO_PIN_11
#define AD9959_RESET_Pin 	GPIO_PIN_12
#define AD9959_PDC_Pin 		GPIO_PIN_8

#define AD9959_SCLK_0 {HAL_GPIO_WritePin(AD9959_SCLK_GPIO, AD9959_SCLK_Pin,  GPIO_PIN_RESET);}
#define AD9959_SCLK_1 {HAL_GPIO_WritePin(AD9959_SCLK_GPIO, AD9959_SCLK_Pin,  GPIO_PIN_SET);}
#define AD9959_SDIO0_0 {HAL_GPIO_WritePin(AD9959_SDIO0_GPIO, AD9959_SDIO0_Pin,  GPIO_PIN_RESET);}
#define AD9959_SDIO0_1 {HAL_GPIO_WritePin(AD9959_SDIO0_GPIO, AD9959_SDIO0_Pin,  GPIO_PIN_SET);}
#define AD9959_CS_0 {HAL_GPIO_WritePin(AD9959_CS_GPIO, AD9959_CS_Pin,  GPIO_PIN_RESET);}
#define AD9959_CS_1 {HAL_GPIO_WritePin(AD9959_CS_GPIO, AD9959_CS_Pin,  GPIO_PIN_SET);}

////AD9959管脚宏定义
//#define RCC_AD9959_CS           RCC_AHB1Periph_GPIOF
//#define RCC_AD9959_SCLK         RCC_AHB1Periph_GPIOC
//#define RCC_AD9959_UPDATE       RCC_AHB1Periph_GPIOF
//// PS0 -- PS3 在扫频模式下使用（扫频模式未调通），点频模式不需要，直接接地
///*
//#define RCC_AD9959_PS0          RCC_AHB1Periph_GPIOF
//#define RCC_AD9959_PS1          RCC_AHB1Periph_GPIOF
//#define RCC_AD9959_PS2          RCC_AHB1Periph_GPIOF
//#define RCC_AD9959_PS3          3
//*/
//#define RCC_AD9959_SDIO0        RCC_AHB1Periph_GPIOC
//// SDIO 在扫频模式下使用（扫频模式未调通），点频模式不需要，直接接地
///*
//#define RCC_AD9959_SDIO1        RCC_AHB1Periph_GPIOC
//#define RCC_AD9959_SDIO2        RCC_AHB1Periph_GPIOA
//#define RCC_AD9959_SDIO3        RCC_AHB1Periph_GPIOC
//*/
//#define RCC_AD9959_PWR          RCC_AHB1Periph_GPIOF
//#define RCC_AD9959_Reset        RCC_AHB1Periph_GPIOF

//#define AD9959_CS_GPIO           GPIOF
//#define AD9959_SCLK_GPIO         GPIOC
//#define AD9959_UPDATE_GPIO       GPIOF
///*
//#define AD9959_PS0_GPIO          GPIOF
//#define AD9959_PS1_GPIO          GPIOF
//#define AD9959_PS2_GPIO          GPIOF
//#define AD9959_PS3_GPIO          GPIOF
//*/
//#define AD9959_SDIO0_GPIO        GPIOC
///*
//#define AD9959_SDIO1_GPIO        GPIOC
//#define AD9959_SDIO2_GPIO        GPIOA
//#define AD9959_SDIO3_GPIO        GPIOC
//*/
//#define AD9959_PWR_GPIO          GPIOF
//#define AD9959_Reset_GPIO        GPIOF

//#define AD9959_CS_Pin           GPIO_Pin_10
//#define AD9959_SCLK_Pin         GPIO_Pin_1
//#define AD9959_UPDATE_Pin       GPIO_Pin_8
///*
//#define AD9959_PS0_Pin          GPIO_Pin_3
//#define AD9959_PS1_Pin          GPIO_Pin_5
//#define AD9959_PS2_Pin          GPIO_Pin_7
//#define AD9959_PS3_Pin          GPIO_Pin_9
//*/
//#define AD9959_SDIO0_Pin        GPIO_Pin_0
///*
//#define AD9959_SDIO1_Pin        GPIO_Pin_2
//#define AD9959_SDIO2_Pin        GPIO_Pin_0
//#define AD9959_SDIO3_Pin        GPIO_Pin_3
//*/
//#define AD9959_PWR_Pin          GPIO_Pin_4
//#define AD9959_Reset_Pin        GPIO_Pin_6

//#define AD9959_CS			    PFout(10)
//#define AD9959_SCLK		        PCout(1)
//#define AD9959_UPDATE	        PFout(8)
///*
//#define AD9959_PS0			    PFout(3)
//#define AD9959_PS1			    PFout(5)
//#define AD9959_PS2			    PFout(7)
//#define AD9959_PS3			    PFout(9)
//*/
//#define AD9959_SDIO0		    PCout(0)
///*
//#define AD9959_SDIO1		    PCout(2)
//#define AD9959_SDIO2		    PAout(0)
//#define AD9959_SDIO3		    PCout(3)
//*/
//#define AD9959_PWR	            PFout(4)
//#define AD9959_Reset		    PFout(6)

typedef struct
{
    uint32_t CurrentFreq[4];
    uint16_t CurrentPhase[4];
    uint16_t CurrentAmp[4];
}AD9959MSG;

extern AD9959MSG AD9959msg;

void AD9959MSGInit(void);

void delay1 (uint32_t length);
void IntReset(void);	        //AD9959复位
void IO_Update(void);           //AD9959更新数据
void Intserve(void);		   //IO口初始化
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData,uint8_t temp);
void Init_AD9959(void);
void Write_frequence(uint8_t Channel,uint32_t Freq);
void WriteFreq(uint8_t Channel,uint32_t Freq,uint8_t mode);
void WriteFreqOrtho(uint32_t Freq);
void Write_Amplitude(uint8_t Channel, uint16_t Ampli);
void WriteAmplitude(uint8_t Channel, uint16_t Ampli, uint8_t mode);
void Write_Phase(uint8_t Channel,uint16_t Phase);
void WritePhase(uint8_t Channel,uint16_t Phase,uint8_t mode);

uint16_t Phase_2_AD9959(float32_t Phase);

#define LINEAR      0
#define FSK         1


void AD9959FreqSweep(uint32_t SFreq,uint32_t EFreq,uint32_t Step,double StayTime,uint8_t mode);
void CalculateFreq(uint32_t Freq, uint8_t buf[4]);
void CalculatePhase(double phase, uint8_t buf[2]);

void CalculateStayTime(double StayTime,uint8_t buf[2]);

#endif
