/**
  ******************************************************************************
  * @file    AD9910.c
  * @author  LMH(2019.8.6)
  * @version V1.0
  * @date    06-August-2019
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
  *
  *-----------适应软排线的定义-----------
  *
  *-------------------------------------------
  *
  * P24 RSO        F3 | P23 PLL       F2
  * P22 PWR        F5 | P21 RST       F4
  * P20 PD         F7 | P19 TEN       F6
  * P18 PF2        F9 | P17 PF1       F8
  * P16 PF0        C0 | P15 SYNC      F10
  * P14 I/O_UP     C2 | P13 OSK       C1
  * P12 DRO        A0 | P11 DRC       C3
  * P10 DRH        A2 | P9  SDIO      A1
  * P8  NC         A4 | P7  SDO       A3
  * P6  NC         A6 | P5  SCK       A5
  * P4  NC         C4 | P3  CSB       A7
  * P2  GND        GND| P1  GND       GND
  *
  *----------------------------------------------
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "AD9910.H"
#include "spi.h"

#define AD9958_CRYSTAL_OSCILLATOR 40000000;

extern uint8_t AD9910_RAM_Data[4096];


AD9910TypeDef Extern_AD9910_Data;
RAM_Mod_Type RAM_Mod_Data;
DRG_Mod_Type DRG_Mod_Data;
OSK_Mod_Type OSK_Mod_Data;
uint8_t Profile_Reg[8]={0x3f,0xff,0x00,0x00,0x25,0x09,0x7b,0x42}; //01振幅控制 23相位控制 4567频率调谐字



uint8_t Ctrl_Func_Reg1[4]={0x00,0x40,0x00,0x00};       //cfr1控制字
uint8_t Ctrl_Func_Reg2[4]={0x01,0x00,0x00,0x00};       //cfr2控制字
uint8_t Ctrl_Func_Reg3[4]={0x05,0x0F,0x41,0x32};       //cfr3控制字  40M输入  25倍频  VC0=101   ICP=001;


void AD9910SetPLLDividerRatio(uint8_t N)
{
	uint8_t Data[4] = {0,0,0,0};
	Extern_AD9910_Data.pll_divider_ratio = N;
	Extern_AD9910_Data.system_clock = Extern_AD9910_Data.pll_divider_ratio * AD9958_CRYSTAL_OSCILLATOR;
	Extern_AD9910_Data._2_32_divide_system_clock_and_pll_divider_ratio = 4294967296.0 / Extern_AD9910_Data.system_clock;
	
	Data[0] = 0x05;
	Data[1] = 0x0F;
	Data[2] = 0x41;
	Data[3] = N<<1;
	
	AD9910WtiteDatas(0x02, Data, 4);
	
}

void AD9910_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	//GPIO时钟使能
	__HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	//输出引脚初始化

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
	GPIO_InitStruct.Pin = AD9910_RST_Pin;
	HAL_GPIO_Init(AD9910_RST_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9910_PF2_Pin;
	HAL_GPIO_Init(AD9910_PF2_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9910_PF0_Pin;
	HAL_GPIO_Init(AD9910_PF0_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9910_IOUP_Pin;
	HAL_GPIO_Init(AD9910_IOUP_Port, &GPIO_InitStruct);	

	GPIO_InitStruct.Pin = AD9910_DRO_Pin;
	HAL_GPIO_Init(AD9910_DRO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9910_DRH_Pin;
	HAL_GPIO_Init(AD9910_DRH_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9910_PWR_Pin;
	HAL_GPIO_Init(AD9910_PWR_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9910_PF1_Pin;
	HAL_GPIO_Init(AD9910_PF1_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9910_OSK_Pin;
	HAL_GPIO_Init(AD9910_OSK_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9910_DRC_Pin;
	HAL_GPIO_Init(AD9910_DRC_Port, &GPIO_InitStruct);
	
//	GPIO_InitStruct.Pin = AD9910_SDIO_Pin;
//	HAL_GPIO_Init(AD9910_SDIO_Port, &GPIO_InitStruct);
//	
//	GPIO_InitStruct.Pin = AD9910_SCK_Pin;
//	HAL_GPIO_Init(AD9910_SCK_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9910_CSB_Pin;
	HAL_GPIO_Init(AD9910_CSB_Port, &GPIO_InitStruct);

	AD9910_PWR_0();
	AD9910_RST_0();
	
	AD9910_PF2_0();
	AD9910_PF1_0();
	AD9910_PF0_0();
	
	AD9910_IO_UPDATE_0();
	AD9910_OSK_0();
	AD9910_DRC_0();			//如果未使用数字斜坡发生器将此引脚与逻辑0相连
	AD9910_DRH_0();			//如果未使用数字斜坡发生器将此引脚与逻辑0相连
//	AD9910_SDIO_0();
//	AD9910_SCK_0();
	AD9910_CSB_1();


}

/**
  * @brief  Initialize AD9910.
  * @param  None
  * @retval None
  */
void AD9910_Init(void)
{
	RAM_Mod_Type RAM_Mod_Data;
	DRG_Mod_Type DRG_Mod_Data;
	OSK_Mod_Type OSK_Mod_Data;
	
	AD9910_IO_Init();
	
	RAM_Mod_Data.State = OFF;
	RAM_Mod_Data.Destin = Frequency;
	RAM_Mod_Data.Ramp_Mod = Bypass;
	
	DRG_Mod_Data.State = OFF;
	DRG_Mod_Data.Destin = Frequency;
	DRG_Mod_Data.Dwell_Mod = No_Dwell;
	
	OSK_Mod_Data.State = OFF;
	OSK_Mod_Data.AutoFunc = OFF;
	OSK_Mod_Data.ExtFunc = OFF;
	
	AD9910_RESET();
	
	AD9910_ModType_Cfig(RAM_Mod_Data, DRG_Mod_Data, OSK_Mod_Data);		//写控制寄存器1、2
	AD9910SetPLLDividerRatio(25);	//40*25 = 1GHz						//写控制寄存器3
	

	
	//HAL_Delay(1);
	
	ProfileSelect(0);
	
}

/**
  * @brief  Write one byte to AD9910 buffer.
  * @param  Data: data (uint8_t) to be send to AD9910
  *         
  * @retval None
  */
//void AD9910WriteByte(uint8_t Data)
//{
//	uint8_t mask;
//	for(mask=0x80; mask; mask>>=1)
//	{
//		AD9910_SCK_0();
//		if(Data & mask){AD9910_SDIO_1();}
//		else {AD9910_SDIO_0();}
//		AD9910_SCK_1();
//	}
//	AD9910_SCK_0();
//	AD9910_SDIO_0();
//	//HAL_Delay(1);
//}

void AD9910WriteByte(uint8_t Data)
{
	HAL_SPI_Transmit_DMA(&hspi1, &Data, 1);//发送数据
}

/**
  * @brief  Write one set of datas to AD9910 buffer.
  * @param  Address: AD9910 Register Address.
  * @param  Data:    Pointer to the datas (uint8_t) to be send to AD9910
  * @param  Length:  The length of datas (uint8_t) to be send to AD9910
  *         
  * @retval None
  */
void AD9910WtiteDatas(uint8_t Address, uint8_t* Data, uint16_t Length)
{
	uint16_t i = 0;
	
	//HAL_Delay(1);
	AD9910_CSB_0();
	
	//AD9910WriteByte(Address);
	HAL_SPI_Transmit_DMA(&hspi1, &Address, 1);
	HAL_SPI_Transmit_DMA(&hspi1, Data, Length);
//	for(i=0;i<Length;i++)
//	{
//		AD9910WriteByte(Data[i]);
//	}
	
	
	AD9910_UPDATE();

}

/**
  * @brief  Write one set of datas to Profile Register X.
  * @param  Order: The order of Profile Register.  Can be 0~7;
  * @param  Data:    Pointer to the datas (uint8_t) to be send to AD9910
  *         
  * @retval None
  */
void AD9910WriteProfileReg(uint16_t Order, uint8_t* Data)
{
	Order %= 8;
	AD9910WtiteDatas(0x0E + Order, Data, 8);
}

/**
  * @brief  Select expected profile register via extern pin PF0~2 of AD9910.
  * @param  Num: The order of Profile Register.  Can be 0~7;
  *
  * @retval None
  */
void ProfileSelect(uint8_t Num)
{
	if(Num & 0x01) 
	{AD9910_PF0_1();}
	else 
	{AD9910_PF0_0();}
	if(Num & 0x02) 
	{AD9910_PF1_1();}
	else 
	{AD9910_PF1_0();}
	if(Num & 0x04) 
	{AD9910_PF2_1();}
	else 
	{AD9910_PF2_0();}
}

/**
  * @brief  Write control word to Ctrl register 1 and Ctrl register 2.
  *         Ctrl register 1 and Ctrl register 2 define the Modulation Type.
  * @param  RAM_Mod_Data: RAM Mod Datas.
  * @param  DRG_Mod_Data: DRG Mod Datas.
  * @param  OSK_Mod_Data: OSK Mod Datas.
  *
  * @retval None
  */

void AD9910_ModType_Cfig(RAM_Mod_Type RAM_Mod_Data, DRG_Mod_Type DRG_Mod_Data, OSK_Mod_Type OSK_Mod_Data)
{
	int i;
	uint8_t Data[4];
	for(i=0;i<4;i++)
	{
		Data[i] = Ctrl_Func_Reg1[i];
	}
	
	Data[0] &=~ ((0x01<<7) | (0x03<<5));
	Data[1] &=~ ((0x01<<7) | (0x0F<<1));
	Data[2] &=~ ((0x01<<1) | 0x01);
	Data[3] &=~ 0x00;
	
	Data[0] |= (RAM_Mod_Data.State<<7) | (RAM_Mod_Data.Destin<<5);
	Data[1] |= (OSK_Mod_Data.ExtFunc<<7) | (RAM_Mod_Data.Ramp_Mod<<1);
	Data[2] |= (OSK_Mod_Data.State<<1) | (OSK_Mod_Data.AutoFunc);
	Data[3] |= 0x00;
	
	AD9910WtiteDatas(0x00, Data, 4);
	
	
	for(i=0;i<4;i++)
	{
		Data[i] = Ctrl_Func_Reg2[i];
	}
	
	Data[0] &=~ 0x00;
	Data[1] &=~ ((0x03<<4) | (1<<3) | (1<<1));
	Data[2] &=~ 0x00;
	Data[3] &=~ 0x00;
	
	Data[0] |= 0x00;
	Data[1] |= (DRG_Mod_Data.Destin<<4) | (DRG_Mod_Data.State<<3) | (DRG_Mod_Data.Dwell_Mod<<1);
	Data[2] |= 0x00;
	Data[3] |= 0x00;
	
	AD9910WtiteDatas(0x01, Data, 4);
	
}
/**
  * @brief  Write amplitude, phase and frequency word to AD9910 profile register X.
  * @param  amplitude: Can be 0~1.
  * @param  phase:     Unit degree.
  * @param  frequency: Unit Hz.
  *
  * @retval None
  */
void AD9910_Amp_Pha_Freq_Set_sig(float amplitude, float phase, float frequency, uint16_t Profile_NUM)
{
	uint16_t amplitude_scale_factor;
	uint16_t phase_offset_word;
	uint32_t frequency_tuning_word;
	//amplitude
	amplitude_scale_factor = amplitude * 16383.0f;
	
	phase_offset_word = phase / 360.0 * 65535.0f;
	
	frequency_tuning_word = frequency * Extern_AD9910_Data._2_32_divide_system_clock_and_pll_divider_ratio;
	
	Profile_Reg[0] = (amplitude_scale_factor>>8) & 0x3F;
	Profile_Reg[1] = amplitude_scale_factor & 0xFF;
	Profile_Reg[2] = (phase_offset_word>>8) & 0xFF;
	Profile_Reg[3] = phase_offset_word & 0xFF;
	Profile_Reg[4] = (frequency_tuning_word>>24) & 0xFF;
	Profile_Reg[5] = (frequency_tuning_word>>16) & 0xFF;
	Profile_Reg[6] = (frequency_tuning_word>>8) & 0xFF;
	Profile_Reg[7] = frequency_tuning_word & 0xFF;
	
	AD9910WriteProfileReg(Profile_NUM, Profile_Reg);
	
}

void AD9910_Amp_Pha_Freq_Set(float amplitude, float phase, float frequency, uint16_t Profile_NUM)
{
	AD9910_Amp_Pha_Freq_Set_sig(amplitude,phase, frequency, Profile_NUM);
	AD9910_Amp_Pha_Freq_Set_sig(amplitude,phase, frequency, Profile_NUM);
	
}

/**
  * @brief  Config RAM parameters.
  * @param  RAM_Mod_Data: RAM Mod Datas.
  *
  * @retval Report illegal RAM address.
  * @attention 不要忘记配合调用AD9910_ModType_Cfig()使能RAM调至模式全局控制位。
  */

uint8_t AD9910_RAM_ProfileCfig(RAM_Mod_Type RAM_Mod_Data)
{
	uint16_t Ramp_Rate;
	float t1,t2,t3,t4;
	if(RAM_Mod_Data.Start_Address>=RAM_Mod_Data.End_Address)
		return 1;
	t1=Extern_AD9910_Data.system_clock/1000000000.0;
	t2=t1/4.0;
	t3=t2*RAM_Mod_Data.Step_Time;
	t4=(uint16_t)t3;
	Ramp_Rate = (uint16_t)(Extern_AD9910_Data.system_clock/1000000000.0/4.0 * RAM_Mod_Data.Step_Time); 
	Profile_Reg[0] = 0x00;
	Profile_Reg[1] = (uint8_t)(Ramp_Rate>>8);
	Profile_Reg[2] = (uint8_t)(Ramp_Rate & 0xFF);
	Profile_Reg[3] = (uint8_t)(RAM_Mod_Data.End_Address>>2);
	Profile_Reg[4] = (uint8_t)(RAM_Mod_Data.End_Address<<6);
	Profile_Reg[5] = (uint8_t)(RAM_Mod_Data.Start_Address>>2);
	Profile_Reg[6] = (uint8_t)(RAM_Mod_Data.Start_Address<<6);
	Profile_Reg[7] = (uint8_t)(RAM_Mod_Data.Operation_Mod | (RAM_Mod_Data.Zero_Crossing<<3) | RAM_Mod_Data.No_Dwell_High<<5);
	
	AD9910WriteProfileReg(RAM_Mod_Data.Profile_ptr, Profile_Reg);
	
	return 0;
}

/**
  * @brief  Load RAM content from AD9910_RAM_Data to AD9910
  * @param  RAM_Mod_Data: RAM Mod Datas.
  * @param  RAMData:      
  * @retval Report illegal RAM address.
  * @attention 强烈建议在调用 AD9910_ModType_Cfig()使能RAM调至模式全局控制位 之前加载RAM值！
  */

uint8_t AD9910_RAM_Load(RAM_Mod_Type RAM_Mod_Data, uint8_t* RAMData)
{
	uint16_t Length;
	if(RAM_Mod_Data.Start_Address>=RAM_Mod_Data.End_Address)
		return 1;
	Length = 4*(RAM_Mod_Data.End_Address - RAM_Mod_Data.Start_Address + 1);
	ProfileSelect(RAM_Mod_Data.Profile_ptr);
	AD9910WtiteDatas(0x16,RAMData,Length);
	
	return 0;
}


//RAM测试程序。
void RamPlayer_test(float time)
{
	//RAM_Mod_Data.Operation_Mod = Bi_Ramp;
	RAM_Mod_Data.Operation_Mod = Recircircle;
	RAM_Mod_Data.Zero_Crossing = OFF;
	RAM_Mod_Data.No_Dwell_High = ON;
	
	RAM_Mod_Data.Start_Address = 0;
	RAM_Mod_Data.End_Address = 63;
	RAM_Mod_Data.Step_Time = time;
	RAM_Mod_Data.Profile_ptr = 0;
	AD9910_RAM_ProfileCfig(RAM_Mod_Data);
	AD9910_RAM_Load(RAM_Mod_Data,AD9910_RAM_Data);
	
	RAM_Mod_Data.State = ON;
	RAM_Mod_Data.Destin = Amplitude;
	RAM_Mod_Data.Ramp_Mod = Bypass;
	AD9910_ModType_Cfig(RAM_Mod_Data, DRG_Mod_Data, OSK_Mod_Data);		//写控制寄存器1、2
	
}





