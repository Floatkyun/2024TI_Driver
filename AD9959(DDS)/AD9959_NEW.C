/*---------------------------------------------------------
modify author： yusnows
file: AD9959.c
version： v0.0
Date；2017.8.7
state： paseed
----------------------------------------------------------*/

#include "AD9959_NEW.H"

AD9959MSG AD9959msg;
uint8_t CSR_DATA0[1] = {0x10};      //开 CH0
uint8_t CSR_DATA1[1] = {0x20};      // 开 CH1
uint8_t CSR_DATA2[1] = {0x40};      // 开 CH2
uint8_t CSR_DATA3[1] = {0x80};      // 开 CH3		
																	
uint8_t FR1_DATA[3] = {0xD0,0x00,0x00};//20倍频 Charge pump control = 75uA FR1<23> -- VCO gain control =0时 system clock below 160 MHz; 
//uint8_t FR1_DATA[3] = {0xD0,0x54,0x00};

//uint8_t FR2_DATA[2] = {0x00,0x00};//default Value = 0x0000
uint8_t FR2_DATA[2] ={0x20,0x00};

uint8_t CFR_DATA[3] = {0x00,0x03,0x02};//default Value = 0x000302	 
//uint8_t CFR_DATA[3] =  {0x80,0xC3,0x00};

																	
uint8_t CPOW0_DATA[2] = {0x00,0x00};//default Value = 0x0000   @ = POW/2^14*360
																	


uint8_t LSRR_DATA[2] = {0x00,0x00};  //default Value = 0x----
//uint8_t LSRR_DATA[4] = {0x00,0x7D,0x00,0x7D};

																	
uint8_t RDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------
//uint8_t RDW_DATA[4] = {0x0,0x00,0x00,0x67};
																	
uint8_t FDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------
//uint8_t FDW_DATA[4] = {0x00,0x00,0x00,0x67};


//FR1_DATA[3] = {0xD0,0x54,0x00}

//FR2_DATA[2] = {0x20,0x00};

//CFR_DATA[3] = {0x80,0xC3,0x00}

//CFTW0_DATA0[4] = {0x00,0x83,0x12,0x6F};

//LSRR_DATA[4] = {0x00,0x7D,0x00,0x7D}

//RDW_DATA[4] = {0x0,0x00,0x00,0x67};

//FDW_DATA[4] = {0x00,0x00,0x00,0x67};

//CW_DATA[4] = {0x33,0x33,0x33,0x33};

//AD9959初始化
void Init_AD9959(void)  
{ 
 
    AD9959MSGInit();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	GPIO_InitStruct.Pin = AD9959_SDIO0_Pin;
	HAL_GPIO_Init(AD9959_SDIO0_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9959_SDIO1_Pin;
	HAL_GPIO_Init(AD9959_SDIO1_GPIO, &GPIO_InitStruct);	
	
	GPIO_InitStruct.Pin = AD9959_SDIO2_Pin;
	HAL_GPIO_Init(AD9959_SDIO2_GPIO, &GPIO_InitStruct);	
	
	GPIO_InitStruct.Pin = AD9959_SDIO3_Pin;
	HAL_GPIO_Init(AD9959_SDIO3_GPIO, &GPIO_InitStruct);		
	
	GPIO_InitStruct.Pin = AD9959_PS0_Pin;
	HAL_GPIO_Init(AD9959_PS0_GPIO, &GPIO_InitStruct);	
	
	GPIO_InitStruct.Pin = AD9959_PS1_Pin;
	HAL_GPIO_Init(AD9959_PS1_GPIO, &GPIO_InitStruct);		
	
	GPIO_InitStruct.Pin = AD9959_PS2_Pin;
	HAL_GPIO_Init(AD9959_PS2_GPIO, &GPIO_InitStruct);			
	
	GPIO_InitStruct.Pin = AD9959_PS3_Pin;
	HAL_GPIO_Init(AD9959_PS3_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9959_SCLK_Pin;
	HAL_GPIO_Init(AD9959_SCLK_GPIO, &GPIO_InitStruct);	

	GPIO_InitStruct.Pin = AD9959_CS_Pin;
	HAL_GPIO_Init(AD9959_CS_GPIO, &GPIO_InitStruct);	

	GPIO_InitStruct.Pin = AD9959_UPDATE_Pin;
	HAL_GPIO_Init(AD9959_UPDATE_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = AD9959_RESET_Pin;
	HAL_GPIO_Init(AD9959_RESET_GPIO, &GPIO_InitStruct);	

	GPIO_InitStruct.Pin = AD9959_PDC_Pin;
	HAL_GPIO_Init(AD9959_PDC_GPIO, &GPIO_InitStruct);	

    Intserve();  //IO口初始化
    IntReset();  //AD9959复位  
    
    //WriteData_AD9959(CFR_ADDR,3,CFR_DATA,1);
    /////////////////////////////////////WriteData_AD9959(FR1_ADDR,3,FR1_DATA,1);//写功能寄存器1
  //WriteData_AD9959(FR2_ADDR,2,FR2_DATA,0);
    //WriteData_AD9959(CFR_ADDR,3,CFR_DATA,1);
//    WriteData_AD9959(CPOW0_ADDR,2,CPOW0_DATA,0);
//  WriteData_AD9959(ACR_ADDR,3,ACR_DATA,0);
  //WriteData_AD9959(LSRR_ADDR,2,LSRR_DATA,0);
  //WriteData_AD9959(RDW_ADDR,2,RDW_DATA,0);
  //WriteData_AD9959(FDW_ADDR,4,FDW_DATA,1);
//写入初始频率
    
//    WriteAmplitude(3, AD9959msg.CurrentAmp[3],0);
//    WriteAmplitude(0, AD9959msg.CurrentAmp[3],0);
//    WriteAmplitude(1, AD9959msg.CurrentAmp[3],0);
//    WriteAmplitude(2, AD9959msg.CurrentAmp[3],0);
//    
//    WriteFreq(3,AD9959msg.CurrentFreq[3],0);
//    WriteFreq(0,AD9959msg.CurrentFreq[0],0); 
//    WriteFreq(1,AD9959msg.CurrentFreq[1],0);
//    WriteFreq(2,AD9959msg.CurrentFreq[2],0);
    
    //IO_Update();
    //IO_Update();  
		
    WriteData_AD9959(0x01U, 3, FR1_DATA, 1);//д????????1
    WriteData_AD9959(0x02U, 2, FR2_DATA, 1);											 
													 
    Intserve();  //IO口初始化
    IntReset();  //AD9959复位 

    WriteData_AD9959(0x01U, 3, FR1_DATA, 1);//??????1
    WriteData_AD9959(0x02U, 2, FR2_DATA, 1);
		
		WriteAmplitude(3, AD9959msg.CurrentAmp[3],0);
    WriteAmplitude(0, AD9959msg.CurrentAmp[3],0);
    WriteAmplitude(1, AD9959msg.CurrentAmp[3],0);
    WriteAmplitude(2, AD9959msg.CurrentAmp[3],0);
    
    WriteFreq(3,AD9959msg.CurrentFreq[3],0);
    WriteFreq(0,AD9959msg.CurrentFreq[0],0); 
    WriteFreq(1,AD9959msg.CurrentFreq[1],0);
    WriteFreq(2,AD9959msg.CurrentFreq[2],0);
		
} 

void AD9959MSGInit(void)
{
    AD9959msg.CurrentFreq[0]=30*MHz;
    AD9959msg.CurrentFreq[1]=30*MHz;
    AD9959msg.CurrentFreq[2]=30*MHz;
    AD9959msg.CurrentFreq[3]=30*MHz;
    AD9959msg.CurrentPhase[0]=0;
    AD9959msg.CurrentPhase[1]=0;
    AD9959msg.CurrentPhase[2]=0;
    AD9959msg.CurrentPhase[3]=0;
    AD9959msg.CurrentAmp[0]=1023;
    AD9959msg.CurrentAmp[1]=1023;
    AD9959msg.CurrentAmp[2]=1023;
    AD9959msg.CurrentAmp[3]=1023;
}
//延时
void delay1 (uint32_t length)
{
    length = length*12;
    while(length--);
}
//IO口初始化
void Intserve(void)		   
{   
	//AD9959_PWR=0;
	HAL_GPIO_WritePin(AD9959_PDC_GPIO, AD9959_PDC_Pin,  GPIO_PIN_RESET);
	
    //AD9959_CS = 1;
	HAL_GPIO_WritePin(AD9959_CS_GPIO, AD9959_CS_Pin,  GPIO_PIN_SET);
	
    //AD9959_SCLK = 0;
	HAL_GPIO_WritePin(AD9959_SCLK_GPIO, AD9959_SCLK_Pin,  GPIO_PIN_RESET);
	
    //AD9959_UPDATE = 0;
		HAL_GPIO_WritePin(AD9959_UPDATE_GPIO, AD9959_UPDATE_Pin,  GPIO_PIN_RESET);
   
    //AD9959_PS0 = 0;
			HAL_GPIO_WritePin(AD9959_PS0_GPIO, AD9959_PS0_Pin,  GPIO_PIN_RESET);
	
    //AD9959_PS1 = 0;
			HAL_GPIO_WritePin(AD9959_PS1_GPIO, AD9959_PS1_Pin,  GPIO_PIN_RESET);	
	
    //AD9959_PS2 = 0;
			HAL_GPIO_WritePin(AD9959_PS2_GPIO, AD9959_PS2_Pin,  GPIO_PIN_RESET);
    //AD9959_PS3 = 0;
			HAL_GPIO_WritePin(AD9959_PS3_GPIO, AD9959_PS3_Pin,  GPIO_PIN_RESET);
    
    //AD9959_SDIO0 = 0;
			HAL_GPIO_WritePin(AD9959_SDIO0_GPIO, AD9959_SDIO0_Pin,  GPIO_PIN_RESET);
    
    //AD9959_SDIO1 = 0;
			HAL_GPIO_WritePin(AD9959_SDIO1_GPIO, AD9959_SDIO1_Pin,  GPIO_PIN_RESET);
			
    //AD9959_SDIO2 = 0;
			HAL_GPIO_WritePin(AD9959_SDIO2_GPIO, AD9959_SDIO2_Pin,  GPIO_PIN_RESET);
			
    //AD9959_SDIO3 = 0;
			HAL_GPIO_WritePin(AD9959_SDIO3_GPIO, AD9959_SDIO3_Pin,  GPIO_PIN_RESET);
    
}
//AD9959复位
void IntReset(void)	  
{
    //AD9959_Reset = 0;
			HAL_GPIO_WritePin(AD9959_RESET_GPIO, AD9959_RESET_Pin,  GPIO_PIN_RESET);
	delay1(1);
	//AD9959_Reset = 1;
			HAL_GPIO_WritePin(AD9959_RESET_GPIO, AD9959_RESET_Pin,  GPIO_PIN_SET);
	delay1(30);
	//AD9959_Reset = 0;
			HAL_GPIO_WritePin(AD9959_RESET_GPIO, AD9959_RESET_Pin,  GPIO_PIN_RESET);
}
 //AD9959更新数据
void IO_Update(void)  
{
	//AD9959_UPDATE = 0;
HAL_GPIO_WritePin(AD9959_UPDATE_GPIO, AD9959_UPDATE_Pin,  GPIO_PIN_RESET);
	delay1(2);
	//AD9959_UPDATE = 1;
HAL_GPIO_WritePin(AD9959_UPDATE_GPIO, AD9959_UPDATE_Pin,  GPIO_PIN_SET);
	delay1(4);
	//AD9959_UPDATE = 0;
HAL_GPIO_WritePin(AD9959_UPDATE_GPIO, AD9959_UPDATE_Pin,  GPIO_PIN_RESET);
}
/*--------------------------------------------
函数功能：控制器通过SPI向AD9959写数据
RegisterAddress: 寄存器地址
NumberofRegisters: 所含字节数
*RegisterData: 数据起始地址
temp: 是否更新IO寄存器
----------------------------------------------*/
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData,uint8_t temp)
{
    uint8_t	ControlValue = 0;
    uint8_t	ValueToWrite = 0;
    uint8_t	RegisterIndex = 0;
    uint8_t	i = 0;

    ControlValue = RegisterAddress;
    //写入地址
    AD9959_SCLK_0;
    AD9959_CS_0;	 
    for(i=0; i<8; i++)
    {
        AD9959_SCLK_0;
        if(0x80 == (ControlValue & 0x80))
				{AD9959_SDIO0_1;	  }
        else
				{ AD9959_SDIO0_0;	  }
        AD9959_SCLK_1;
        ControlValue <<= 1;
    }
    AD9959_SCLK_0;
    //写入数据
    for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
    {
        ValueToWrite = RegisterData[RegisterIndex];
        for (i=0; i<8; i++)
        {
            AD9959_SCLK_0;
            if(0x80 == (ValueToWrite & 0x80))
						{AD9959_SDIO0_1;}	  
            else
						{AD9959_SDIO0_0;	}  
            AD9959_SCLK_1;
            ValueToWrite <<= 1;
        }
        AD9959_SCLK_0;		
    }	
    if(temp==1)
        IO_Update();	
  AD9959_CS_1;
} 
//void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData,uint8_t temp)
//{

//HAL_SPI_Transmit_DMA(&hspi1,txbuff,rxbuff,8);

//}
/*---------------------------------------
函数功能：设置通道输出频率
Channel:  输出通道
Freq:     输出频率
---------------------------------------*/
void Write_frequence(uint8_t Channel,uint32_t Freq)
{	 
    uint8_t CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
    uint32_t Temp;            
    Temp=(uint32_t)Freq*8.589934592;	   //将输入频率因子分为四个字节  4.294967296=(2^32)/500000000
    CFTW0_DATA[3]=(uint8_t)Temp;
    CFTW0_DATA[2]=(uint8_t)(Temp>>8);
    CFTW0_DATA[1]=(uint8_t)(Temp>>16);
    CFTW0_DATA[0]=(uint8_t)(Temp>>24);
    if(Channel==0)	  
    {
        AD9959msg.CurrentFreq[0]=Freq;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA0,1);//控制寄存器写入CH0通道
        WriteData_AD9959(CFTW0_ADDR,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH0设定频率
    }
    else if(Channel==1)	
    {
        AD9959msg.CurrentFreq[1]=Freq;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA1,1);//控制寄存器写入CH1通道
        WriteData_AD9959(CFTW0_ADDR,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH1设定频率	
    }
    else if(Channel==2)	
    {
        AD9959msg.CurrentFreq[2]=Freq;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA2,1);    //控制寄存器写入CH2通道
        WriteData_AD9959(CFTW0_ADDR,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH2设定频率	
    }
    else if(Channel==3)	
    {
        AD9959msg.CurrentFreq[3]=Freq;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA3,1);    //控制寄存器写入CH3通道
        WriteData_AD9959(CFTW0_ADDR,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH3设定频率	
    }																																																																										 

} 


/*---------------------------------------
函数功能：设置通道输出频率
Channel:  输出通道
Freq:     输出频率
mode:   0: 不立即更新寄存器， 1: 立即更新
---------------------------------------*/

void WriteFreq(uint8_t Channel,uint32_t Freq,uint8_t mode)
{
    uint8_t CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
    uint32_t Temp;            
    Temp=(uint32_t)Freq*8.589934592;	   //将输入频率因子分为四个字节  8.589934592=(2^32)/500000000
    CFTW0_DATA[3]=(uint8_t)Temp;
    CFTW0_DATA[2]=(uint8_t)(Temp>>8);
    CFTW0_DATA[1]=(uint8_t)(Temp>>16);
    CFTW0_DATA[0]=(uint8_t)(Temp>>24);
    if(Channel==0)	  
    {
        AD9959msg.CurrentFreq[0]=Freq;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA0,mode);//控制寄存器写入CH0通道
        WriteData_AD9959(CFTW0_ADDR,4,CFTW0_DATA,mode);//CTW0 address 0x04.输出CH0设定频率
    }
    else if(Channel==1)	
    {
        AD9959msg.CurrentFreq[1]=Freq;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA1,mode);//控制寄存器写入CH1通道
        WriteData_AD9959(CFTW0_ADDR,4,CFTW0_DATA,mode);//CTW0 address 0x04.输出CH1设定频率	
    }
    else if(Channel==2)	
    {
        AD9959msg.CurrentFreq[2]=Freq;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA2,mode);    //控制寄存器写入CH2通道
        WriteData_AD9959(CFTW0_ADDR,4,CFTW0_DATA,mode);//CTW0 address 0x04.输出CH2设定频率	
    }
    else if(Channel==3)	
    {
        AD9959msg.CurrentFreq[3]=Freq;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA3,mode);    //控制寄存器写入CH3通道
        WriteData_AD9959(CFTW0_ADDR,4,CFTW0_DATA,mode);//CTW0 address 0x04.输出CH3设定频率	
    }	
}

void WriteFreqOrtho(uint32_t Freq)
{
    uint8_t CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
    uint32_t Temp;
    uint16_t A_temp;//=0x23ff;
    uint8_t ACR_DATA[3] = {0x00,0x00,0x00};//default Value = 0x--0000 Rest = 18.91/Iout 
    uint8_t csr[1]={0xF0};
    uint64_t  temp=Freq;
    
    if(Freq>160*MHz)
        Freq=160*MHz;
    
    AD9959msg.CurrentFreq[0]=AD9959msg.CurrentFreq[1]=AD9959msg.CurrentFreq[2]=AD9959msg.CurrentFreq[3]=Freq;
    
    A_temp=(680+(temp*258)/(160*MHz))|0x1000;       //765
    if(temp>=145*MHz)
        A_temp=A_temp+(temp*35)/(160*MHz);
    if(temp>=152*MHz)
        A_temp=A_temp+(temp*20)/(160*MHz);
    if(temp>=155*MHz)
        A_temp=A_temp+(temp*30)/(160*MHz);
    ACR_DATA[2] = (uint8_t)A_temp;       //低位数据
    ACR_DATA[1] = (uint8_t)(A_temp>>8); //高位数据
    Temp=(uint32_t)Freq*8.589934592;	   //将输入频率因子分为四个字节  8.589934592=(2^32)/500000000
    CFTW0_DATA[3]=(uint8_t)Temp;
    CFTW0_DATA[2]=(uint8_t)(Temp>>8);
    CFTW0_DATA[1]=(uint8_t)(Temp>>16);
    CFTW0_DATA[0]=(uint8_t)(Temp>>24);
    
    if(AD9959msg.CurrentPhase[0]!=0||AD9959msg.CurrentPhase[1]!=0||AD9959msg.CurrentPhase[2]!=4096*2||AD9959msg.CurrentPhase[3]!=4096*3)
    {
        WritePhase(0,0,0);
        WritePhase(1,4096,0);
        WritePhase(2,4096*2,0);
        WritePhase(3,4096*3,0);
        AD9959msg.CurrentPhase[0]=0;
        AD9959msg.CurrentPhase[1]=4096;
        AD9959msg.CurrentPhase[2]=4096*2;
        AD9959msg.CurrentPhase[3]=4096*3;
    }
    
    WriteData_AD9959(CSR_ADDR,1,csr,0);
    
    WriteData_AD9959(ACR_ADDR,3,ACR_DATA,0);
    
    WriteData_AD9959(CFTW0_ADDR,4,CFTW0_DATA,0);
    
    IO_Update();
}
/*---------------------------------------
函数功能：设置通道输出幅度
Channel:  输出通道
Ampli:    输出幅度
---------------------------------------*/
void Write_Amplitude(uint8_t Channel, uint16_t Ampli)
{ 
    uint16_t A_temp;//=0x23ff;
    uint8_t ACR_DATA[3] = {0x00,0x00,0x00};//default Value = 0x--0000 Rest = 18.91/Iout 

    A_temp=Ampli|0x1000;
    ACR_DATA[2] = (uint8_t)A_temp;       //低位数据
    ACR_DATA[1] = (uint8_t)(A_temp>>8); //高位数据
    if(Channel==0)
    {
        AD9959msg.CurrentAmp[0]=Ampli;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA0,1); 
        WriteData_AD9959(ACR_ADDR,3,ACR_DATA,1); 
    }
    else if(Channel==1)
    {
        AD9959msg.CurrentAmp[1]=Ampli;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA1,1); 
        WriteData_AD9959(ACR_ADDR,3,ACR_DATA,1);
    }
    else if(Channel==2)
    {
        AD9959msg.CurrentAmp[2]=Ampli;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA2,1); 
        WriteData_AD9959(ACR_ADDR,3,ACR_DATA,1); 
    }
    else if(Channel==3)
    {
        AD9959msg.CurrentAmp[3]=Ampli;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA3,1); 
        WriteData_AD9959(ACR_ADDR,3,ACR_DATA,1); 
    }
}
/*---------------------------------------
函数功能：设置通道输出幅度
对应关系： 25mv/100 大概线性
Channel:  输出通道
Ampli:    输出幅度
mode :   0:       1:
---------------------------------------*/
void WriteAmplitude(uint8_t Channel, uint16_t Ampli, uint8_t mode)
{
    uint16_t A_temp;//=0x23ff;
    uint8_t ACR_DATA[3] = {0x00,0x00,0x00};//default Value = 0x--0000 Rest = 18.91/Iout 

    A_temp=Ampli|0x1000;
    ACR_DATA[2] = (uint8_t)A_temp;       //低位数据
    ACR_DATA[1] = (uint8_t)(A_temp>>8); //高位数据
    if(Channel==0)
    {
        AD9959msg.CurrentAmp[0]=Ampli;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA0,mode); 
        WriteData_AD9959(ACR_ADDR,3,ACR_DATA,mode); 
    }
    else if(Channel==1)
    {
        AD9959msg.CurrentAmp[1]=Ampli;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA1,mode); 
        WriteData_AD9959(ACR_ADDR,3,ACR_DATA,mode);
    }
    else if(Channel==2)
    {
        AD9959msg.CurrentAmp[2]=Ampli;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA2,mode); 
        WriteData_AD9959(ACR_ADDR,3,ACR_DATA,mode); 
    }
    else if(Channel==3)
    {
        AD9959msg.CurrentAmp[3]=Ampli;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA3,mode); 
        WriteData_AD9959(ACR_ADDR,3,ACR_DATA,mode); 
    }    
}



uint16_t Phase_2_AD9959(float32_t Phase)
{
	Phase=fmodf(Phase,360.0);
	if(Phase<0)
	{
		Phase=360.0+Phase;
	}
	return Phase*16383.0/360.0;
}

/*---------------------------------------
函数功能：设置通道输出相位
Channel:  输出通道
Phase:    输出相位,范围：0~16383(对应角度：0°~360°)
---------------------------------------*/
void Write_Phase(uint8_t Channel,uint16_t Phase)
{
    uint16_t P_temp=0;
    P_temp=(uint16_t)Phase;
    CPOW0_DATA[1]=(uint8_t)P_temp;
    CPOW0_DATA[0]=(uint8_t)(P_temp>>8);
    if(Channel==0)
    {
        AD9959msg.CurrentPhase[3]=Phase;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA0,0); 
        WriteData_AD9959(CPOW0_ADDR,2,CPOW0_DATA,0);
    }
    else if(Channel==1)
    {
        AD9959msg.CurrentPhase[3]=Phase;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA1,0); 
        WriteData_AD9959(CPOW0_ADDR,2,CPOW0_DATA,0);
    }
    else if(Channel==2)
    {
        AD9959msg.CurrentPhase[3]=Phase;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA2,0); 
        WriteData_AD9959(CPOW0_ADDR,2,CPOW0_DATA,0);
    }
    else if(Channel==3)
    {
        AD9959msg.CurrentPhase[3]=Phase;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA3,0); 
        WriteData_AD9959(CPOW0_ADDR,2,CPOW0_DATA,0);
    }
}	 

/*---------------------------------------
函数功能：设置通道输出相位
Channel:  输出通道
Phase:    输出相位,范围：0~16383(对应角度：0°~360°)
mode :    0:不立即更新， 1:立即更新
---------------------------------------*/
void WritePhase(uint8_t Channel,uint16_t Phase, uint8_t mode)
{
    uint16_t P_temp=0;
    P_temp=(uint16_t)Phase;
    CPOW0_DATA[1]=(uint8_t)P_temp;
    CPOW0_DATA[0]=(uint8_t)(P_temp>>8);
    if(Channel==0)
    {
        AD9959msg.CurrentPhase[0]=Phase;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA0,mode); 
        WriteData_AD9959(CPOW0_ADDR,2,CPOW0_DATA,mode);
    }
    else if(Channel==1)
    {
        AD9959msg.CurrentPhase[1]=Phase;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA1,mode); 
        WriteData_AD9959(CPOW0_ADDR,2,CPOW0_DATA,mode);
    }
    else if(Channel==2)
    {
        AD9959msg.CurrentPhase[2]=Phase;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA2,mode); 
        WriteData_AD9959(CPOW0_ADDR,2,CPOW0_DATA,mode);
    }
    else if(Channel==3)
    {
        AD9959msg.CurrentPhase[3]=Phase;
        WriteData_AD9959(CSR_ADDR,1,CSR_DATA3,mode); 
        WriteData_AD9959(CPOW0_ADDR,2,CPOW0_DATA,mode);
    }
}

/////////////////////////////////////////////////
//function AD9959FreqSweep
//test not pass
/////////////////////////////////////////////////
void AD9959FreqSweep(uint32_t SFreq,uint32_t EFreq,uint32_t Step,double StayTime,uint8_t mode)
{
    uint8_t tempCSR[1]={0xF0};
    uint8_t tempCFR[3]={0x80,0xE3,0x02};
//    uint8_t tempFR1[3]={0xD0,0x00,0x00};
    uint8_t tempCFTWS[4]={0x00,0x00,0x00,0x00};
    uint8_t tempCFTWE[4]={0x00,0x00,0x00,0x00};
    uint8_t tempRDW[4]={0x00,0x00,0x00,0x00};
    uint8_t tempFDW[4]={0x00,0x00,0x00,0x00};
    uint8_t tempLSRR[2]={0xFF,0xFF}; //LSRR[15:8] is Falling sweep ramp rate,LSRR[7:0] is Rising sweep ramp rate
    CalculateFreq(SFreq,tempCFTWS);
    CalculateFreq(EFreq,tempCFTWE);
    CalculateFreq(Step,tempRDW);
    CalculateFreq(Step,tempFDW);
    CalculateStayTime(StayTime,tempLSRR);
    WritePhase(0,0,0);
    WritePhase(1,4096,0);
    WritePhase(2,4096*2,0);
    WritePhase(3,4096*3,0);
    IO_Update();
    if(mode==LINEAR)
    {
        WriteData_AD9959(CSR_ADDR,1,tempCSR,0);
        WriteData_AD9959(CFR_ADDR,3,tempCFR,0);
        WriteData_AD9959(FREQS0_ADDR,4,tempCFTWS,0);
        WriteData_AD9959(FREQE0_ADDR,4,tempCFTWE,0);
        WriteData_AD9959(RDW_ADDR,4,tempRDW,0);
        WriteData_AD9959(FDW_ADDR,4,tempFDW,0);
        WriteData_AD9959(LSRR_ADDR,2,tempLSRR,0);
        
        IO_Update();
/*        AD9959_PS0=1;*/
    }
    else if(mode==FSK)
    {
        
    }
}


void CalculateFreq(uint32_t Freq, uint8_t buf[4])
{
    uint32_t Temp;           
    Temp=(uint32_t)Freq*8.589934592;	   //将输入频率因子分为四个字节  8.589934592=(2^32)/500000000
    buf[3]=(uint8_t)Temp;
    buf[2]=(uint8_t)(Temp>>8);
    buf[1]=(uint8_t)(Temp>>16);
    buf[0]=(uint8_t)(Temp>>24);
}


void CalculatePhase(double phase, uint8_t buf[2])
{
    uint16_t P_temp=0;
    P_temp=(uint16_t)phase;
    buf[1]=(uint8_t)P_temp;
    buf[0]=(uint8_t)(P_temp>>8);
}

/////////////////////////////////////////////////////
//StayTime: unit: ns, max: 2040ns, min: 8ns, When SYSCLK=500MHz;
/////////////////////////////////////////////////////
void CalculateStayTime(double StayTime,uint8_t buf[2])
{
    double tempSysClk=SYSCLK/4;
    uint16_t LSRR=0;
    if(StayTime>2048)
        StayTime=2040;
    if(StayTime<8)
        StayTime=8;
    LSRR=StayTime/8;
    buf[0]=LSRR;
    buf[1]=LSRR;
}

