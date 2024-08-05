# 2024TI_Driver
电赛高频方向常用模块的STM32单片机驱动（基于HAL库），包括AD9910、AD9959、PE4302、SI5351   
你可以将其移植到 <em>**STM32 F1、F4、F7**</em> 系列的单片机上，并配合CUBEMX开发   
以下列出这些驱动的基本使用示例

- ## AD9910驱动使用示例 
初始化AD9910，并设置设置发生正弦信号的增益为0.64、相位为0、频率为30MHz：  

    AD9910_Init();//初始化AD9910
    AD9910_Amp_Pha_Freq_Set(float Gain,float Phase,float Freq,0);  //设置发生波形的增益（取值为0~1之间的小数）、相位（单位为° 小数）、频率（单位Hz）
    AD9910_Amp_Pha_Freq_Set(0.64,0.0,30e6,0);                      //设置发生波形的增益为0.64、相位为0、频率为30MHz
    
- ## AD9959驱动使用示例   
原型函数：   

    Write_frequence(uint8_t Chanel,uint32_t Freq);
		Write_Amplitude(uint8_t Chanel,uint16_t Gain);
		Write_Phase(uint8_t Chanel,uint16_t Phase);
  
其中`Chanel`为通道序号 取值0\~3   
 `Freq`为频率 单位为Hz 取值0\~50MHz   
 `Gain`为增益 取值 0\~1023   
 `Phase`为相位 取值 0\~16383 对应0\~360°   

--------------------------------------------------------
   
初始化AD9959，并设置通道0发生正弦信号的增益为512、相位为0、频率为30MHz：  

    Init_AD9959();   
    Write_frequence(0,30e6);   
	  Write_Amplitude(0, 512);   
	  Write_Phase(0,0);  

- ## PE4302驱动使用示例:    
初始化PE4302，设置衰减为`-Gain dB`:   

    PE43xx_init();
    PE43xx_changelevel(Gain);



- ## SI5351驱动使用示例

在SI5351硬件模块没有连接到单片机上时，请在主函数中去除所有该驱动相关函数 **否则一定会引起单片机卡死！**  
（硬件IIC特性，单片机等待IIC通信应答位所致）   

本模块在实际使用时会遇到很多问题，请在出现问题时仔细查看[`si5351.c`](https://github.com/Floatkyun/2024TI_Driver/blob/main/SI5351(%E6%97%B6%E9%92%9F%E8%8A%AF%E7%89%87)/si5351.c)中对函数所接受参数的限制。   

初始化SI5351，并设置通道0发出16MHz时钟：

    si5351_Init();                                     //初始化SI5351
    si5351_setupPLLInt(SI5351_PLL_A, 32);              // 25MHz crystal osc * 32 = 800 MHz
    si5351_setupMultisynthInt(0, SI5351_PLL_A, 50);    // 800MHz / 50 = 16Mhz
    si5351_setupRdiv(0, SI5351_R_DIV_1);               // 16Mhz/1=16Mhz 
    si5351_enableOutputs(0xFF);                        // 使能时钟输出
初始化SI5351，并设置通道2发出32.768KHz时钟：

    si5351_Init();                                          //初始化SI5351
    si5351_setupPLL(SI5351_PLL_B, 28, 7012, 390625);        // 25MHz crystal osc * (28 + (7012/390625)) = 700.448768 MHz
    si5351_setupMultisynth(2, SI5351_PLL_B, 1336, 0, 1);    // 700.448768 MHz/ (1336 + (0 / 1)) = 0.524288 MHz
    si5351_setupRdiv(2, SI5351_R_DIV_16);                   // 0.524288 MHz/ 16 = 0.032768 MHz = 32.768KHz
    si5351_enableOutputs(0xFF);                             // 使能时钟输出
实际使用中，初始化函数 `si5351_Init()` 只使用一次即可，初始化将重置模块之前的设置   
实际使用中，使能输出函数 `si5351_enableOutputs(0xFF)` 只使用一次即可


