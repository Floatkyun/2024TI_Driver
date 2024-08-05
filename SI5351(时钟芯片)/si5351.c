/**************************************************************************/
/*!
    @file     si5351.c

    @author   K. Townsend (Adafruit Industries)

    @brief    Driver for the SI5351 160MHz Clock Gen

    @section  REFERENCES

    Si5351A/B/C Datasheet:
    http://www.silabs.com/Support%20Documents/TechnicalDocs/Si5351.pdf

    Manually Generating an Si5351 Register Map:
    http://www.silabs.com/Support%20Documents/TechnicalDocs/AN619.pdf

    @section  LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2014, Adafruit Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#include "main.h"
// CHANGE THIS FOR YOUR CHIP
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include <math.h>
#include <si5351.h>

/**************************************************************************/
/*!
    Initializes I2C and configures the breakout (call this function before
    doing anything else)
*/
/**************************************************************************/
err_t si5351_Init(void)
{

	/*!
	    Constructor
	*/
	  m_si5351Config.initialised     = 0;
	  m_si5351Config.crystalFreq     = SI5351_CRYSTAL_FREQ_25MHZ;
	  m_si5351Config.crystalLoad     = SI5351_CRYSTAL_LOAD_10PF;
	  m_si5351Config.crystalPPM      = 30;
	  m_si5351Config.plla_configured = 0;
	  m_si5351Config.plla_freq       = 0;
	  m_si5351Config.pllb_configured = 0;
	  m_si5351Config.pllb_freq       = 0;
	  m_si5351Config.ms0_freq		 = 0;
	  m_si5351Config.ms1_freq		 = 0;
	  m_si5351Config.ms2_freq		 = 0;
	  m_si5351Config.ms0_r_div		 = 0;
	  m_si5351Config.ms1_r_div		 = 0;
	  m_si5351Config.ms2_r_div		 = 0;



  /* Disable all outputs setting CLKx_DIS high */
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, 0xFF));

  /* Power down all output drivers */
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_16_CLK0_CONTROL, 0x80));
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_17_CLK1_CONTROL, 0x80));
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_18_CLK2_CONTROL, 0x80));
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_19_CLK3_CONTROL, 0x80));
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_20_CLK4_CONTROL, 0x80));
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_21_CLK5_CONTROL, 0x80));
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_22_CLK6_CONTROL, 0x80));
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_23_CLK7_CONTROL, 0x80));

  /* Set the load capacitance for the XTAL */
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_183_CRYSTAL_INTERNAL_LOAD_CAPACITANCE,
                       m_si5351Config.crystalLoad));

  /* Set interrupt masks as required (see Register 2 description in AN619).
     By default, ClockBuilder Desktop sets this register to 0x18.
     Note that the least significant nibble must remain 0x8, but the most
     significant nibble may be modified to suit your needs. */

  /* Reset the PLL config fields just in case we call init again */
  m_si5351Config.plla_configured = 0;
  m_si5351Config.plla_freq = 0;
  m_si5351Config.pllb_configured = 0;
  m_si5351Config.pllb_freq = 0;

  /* All done! */
  m_si5351Config.initialised = 1;

  return ERROR_NONE;
}


/**************************************************************************/
/*!
  @brief  Sets the multiplier for the specified PLL using integer values

  @param  pll   The PLL to configure, which must be one of the following:
                - SI5351_PLL_A
                - SI5351_PLL_B
  @param  mult  The PLL integer multiplier (must be between 15 and 90)
*/
/**************************************************************************/
err_t si5351_setupPLLInt(si5351PLL_t pll, uint8_t mult)
{
  return si5351_setupPLL(pll, mult, 0, 1);
}

/**************************************************************************/
/*!
    @brief  Sets the multiplier for the specified PLL

    @param  pll   The PLL to configure, which must be one of the following:
                  - SI5351_PLL_A
                  - SI5351_PLL_B
    @param  mult  The PLL integer multiplier (must be between 15 and 90)
    @param  num   The 20-bit numerator for fractional output (0..1,048,575).
                  Set this to '0' for integer output.
    @param  denom The 20-bit denominator for fractional output (1..1,048,575).
                  Set this to '1' or higher to avoid divider by zero errors.

    @section PLL Configuration

    fVCO is the PLL output, and must be between 600..900MHz, where:

        fVCO = fXTAL * (a+(b/c))

    fXTAL = the crystal input frequency
    a     = an integer between 15 and 90
    b     = the fractional numerator (0..1,048,575)
    c     = the fractional denominator (1..1,048,575)

    NOTE: Try to use integers whenever possible to avoid clock jitter
    (only use the a part, setting b to '0' and c to '1').

    See: http://www.silabs.com/Support%20Documents/TechnicalDocs/AN619.pdf
*/
/**************************************************************************/
err_t si5351_setupPLL(si5351PLL_t pll,
                                uint8_t     mult,
                                uint32_t    num,
                                uint32_t    denom)
{
  uint32_t P1;       /* PLL config register P1 */
  uint32_t P2;	     /* PLL config register P2 */
  uint32_t P3;	     /* PLL config register P3 */

  /* Basic validation */
  ASSERT( m_si5351Config.initialised, ERROR_DEVICENOTINITIALISED );
  ASSERT( (mult > 14) && (mult < 91), ERROR_INVALIDPARAMETER ); /* mult = 15..90 */
  ASSERT( denom > 0,                  ERROR_INVALIDPARAMETER ); /* Avoid divide by zero */
  ASSERT( num <= 0xFFFFF,             ERROR_INVALIDPARAMETER ); /* 20-bit limit */
  ASSERT( denom <= 0xFFFFF,           ERROR_INVALIDPARAMETER ); /* 20-bit limit */

  /* Feedback Multisynth Divider Equation
   *
   * where: a = mult, b = num and c = denom
   *
   * P1 register is an 18-bit value using following formula:
   *
   * 	P1[17:0] = 128 * mult + floor(128*(num/denom)) - 512
   *
   * P2 register is a 20-bit value using the following formula:
   *
   * 	P2[19:0] = 128 * num - denom * floor(128*(num/denom))
   *
   * P3 register is a 20-bit value using the following formula:
   *
   * 	P3[19:0] = denom
   */

  /* Set the main PLL config registers */
  if (num == 0)
  {
    /* Integer mode */
    P1 = 128 * mult - 512;
    P2 = num;
    P3 = denom;
  }
  else
  {
    /* Fractional mode */
    P1 = (uint32_t)(128 * mult + floor(128 * ((float)num/(float)denom)) - 512);
    P2 = (uint32_t)(128 * num - denom * floor(128 * ((float)num/(float)denom)));
    P3 = denom;
  }

  /* Get the appropriate starting point for the PLL registers */
  uint8_t baseaddr = (pll == SI5351_PLL_A ? 26 : 34);

  /* The datasheet is a nightmare of typos and inconsistencies here! */
  ASSERT_STATUS( si5351_write8( baseaddr,   (P3 & 0x0000FF00) >> 8));
  ASSERT_STATUS( si5351_write8( baseaddr+1, (P3 & 0x000000FF)));
  ASSERT_STATUS( si5351_write8( baseaddr+2, (P1 & 0x00030000) >> 16));
  ASSERT_STATUS( si5351_write8( baseaddr+3, (P1 & 0x0000FF00) >> 8));
  ASSERT_STATUS( si5351_write8( baseaddr+4, (P1 & 0x000000FF)));
  ASSERT_STATUS( si5351_write8( baseaddr+5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16) ));
  ASSERT_STATUS( si5351_write8( baseaddr+6, (P2 & 0x0000FF00) >> 8));
  ASSERT_STATUS( si5351_write8( baseaddr+7, (P2 & 0x000000FF)));

  /* Reset both PLLs */
  ASSERT_STATUS( si5351_write8(SI5351_REGISTER_177_PLL_RESET, (1<<7) | (1<<5) ));

  /* Store the frequency settings for use with the Multisynth helper */
  if (pll == SI5351_PLL_A)
  {
    float fvco = m_si5351Config.crystalFreq * (mult + ( (float)num / (float)denom ));
    m_si5351Config.plla_configured = 1; //true
    m_si5351Config.plla_freq = (uint32_t)floor(fvco);
  }
  else
  {
    float fvco = m_si5351Config.crystalFreq * (mult + ( (float)num / (float)denom ));
    m_si5351Config.pllb_configured = 1; //true
    m_si5351Config.pllb_freq = (uint32_t)floor(fvco);
  }

  return ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Configures the Multisynth divider using integer output.

    @param  output    The output channel to use (0..2)
    @param  pllSource	The PLL input source to use, which must be one of:
                      - SI5351_PLL_A
                      - SI5351_PLL_B
    @param  div       The integer divider for the Multisynth output,
                      which must be one of the following values:
                      - SI5351_MULTISYNTH_DIV_4
                      - SI5351_MULTISYNTH_DIV_6
                      - SI5351_MULTISYNTH_DIV_8
*/
/**************************************************************************/
err_t si5351_setupMultisynthInt(uint8_t               output,
                                          si5351PLL_t           pllSource,
                                          si5351MultisynthDiv_t div)
{
  return si5351_setupMultisynth(output, pllSource, div, 0, 1);
}


err_t si5351_setupRdiv(uint8_t  output, si5351RDiv_t div) {
  ASSERT( output < 3,                 ERROR_INVALIDPARAMETER);  /* Channel range */
  
  uint8_t Rreg, regval, rDiv;

  if (output == 0) Rreg = SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3;
  if (output == 1) Rreg = SI5351_REGISTER_52_MULTISYNTH1_PARAMETERS_3;
  if (output == 2) Rreg = SI5351_REGISTER_60_MULTISYNTH2_PARAMETERS_3;

  si5351_read8(Rreg, &regval);

  regval &= 0x0F;
  uint8_t divider = div;
  divider &= 0x07;
  divider <<= 4;
  regval |= divider;
  si5351_write8(Rreg, regval);

  switch(div)
  {
  case 0:
  rDiv = 1;
  break;

  case 1:
  rDiv = 2;
  break;

  case 2:
  rDiv = 4;
  break;

  case 3:
  rDiv = 8;
  break;

  case 4:
  rDiv = 16;
  break;

  case 5:
  rDiv = 32;
  break;

  case 6:
  rDiv = 64;
  break;

  case 7:
  rDiv = 128;
  break;
  }

  switch(output)
  {
  case 0:
  m_si5351Config.ms0_r_div = rDiv;
  break;

  case 1:
  m_si5351Config.ms1_r_div = rDiv;
  break;

  case 2:
  m_si5351Config.ms2_r_div = rDiv;
  break;
  }

  return ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Configures the Multisynth divider, which determines the
            output clock frequency based on the specified PLL input.

    @param  output    The output channel to use (0..2)
    @param  pllSource	The PLL input source to use, which must be one of:
                      - SI5351_PLL_A
                      - SI5351_PLL_B
    @param  div       The integer divider for the Multisynth output.
                      If pure integer values are used, this value must
                      be one of:
                      - SI5351_MULTISYNTH_DIV_4
                      - SI5351_MULTISYNTH_DIV_6
                      - SI5351_MULTISYNTH_DIV_8
                      If fractional output is used, this value must be
                      between 8 and 900.
    @param  num       The 20-bit numerator for fractional output
                      (0..1,048,575). Set this to '0' for integer output.
    @param  denom     The 20-bit denominator for fractional output
                      (1..1,048,575). Set this to '1' or higher to
                      avoid divide by zero errors.

    @section Output Clock Configuration

    The multisynth dividers are applied to the specified PLL output,
    and are used to reduce the PLL output to a valid range (500kHz
    to 160MHz). The relationship can be seen in this formula, where
    fVCO is the PLL output frequency and MSx is the multisynth
    divider:

        fOUT = fVCO / MSx

    Valid multisynth dividers are 4, 6, or 8 when using integers,
    or any fractional values between 8 + 1/1,048,575 and 900 + 0/1

    The following formula is used for the fractional mode divider:

        a + b / c

    a = The integer value, which must be 4, 6 or 8 in integer mode (MSx_INT=1)
        or 8..900 in fractional mode (MSx_INT=0).
    b = The fractional numerator (0..1,048,575)
    c = The fractional denominator (1..1,048,575)

    @note   Try to use integers whenever possible to avoid clock jitter

    @note   For output frequencies > 150MHz, you must set the divider
            to 4 and adjust to PLL to generate the frequency (for example
            a PLL of 640 to generate a 160MHz output clock). This is not
            yet supported in the driver, which limits frequencies to
            500kHz .. 150MHz.

    @note   For frequencies below 500kHz (down to 8kHz) Rx_DIV must be
            used, but this isn't currently implemented in the driver.
*/
/**************************************************************************/
err_t si5351_setupMultisynth(uint8_t     output,
                                       si5351PLL_t pllSource,
                                       uint32_t    div,
                                       uint32_t    num,
                                       uint32_t    denom)
{
  uint32_t P1;       /* Multisynth config register P1 */
  uint32_t P2;	     /* Multisynth config register P2 */
  uint32_t P3;	     /* Multisynth config register P3 */

  /* Basic validation */
  ASSERT( m_si5351Config.initialised, ERROR_DEVICENOTINITIALISED);
  ASSERT( output < 3,                 ERROR_INVALIDPARAMETER);  /* Channel range */
  //ASSERT( div > 3,                    ERROR_INVALIDPARAMETER);  /* Divider integer value */
  //ASSERT( div < 901,                  ERROR_INVALIDPARAMETER);  /* Divider integer value */
  //ASSERT( denom > 0,                  ERROR_INVALIDPARAMETER ); /* Avoid divide by zero */
  //ASSERT( num <= 0xFFFFF,             ERROR_INVALIDPARAMETER ); /* 20-bit limit */
  //ASSERT( denom <= 0xFFFFF,           ERROR_INVALIDPARAMETER ); /* 20-bit limit */


  /* Make sure the requested PLL has been initialised */
  if (pllSource == SI5351_PLL_A)
  {
    ASSERT(m_si5351Config.plla_configured = 1, ERROR_INVALIDPARAMETER);
  }
  else
  {
    ASSERT(m_si5351Config.pllb_configured = 1, ERROR_INVALIDPARAMETER);
  }

  /* Output Multisynth Divider Equations
   *
   * where: a = div, b = num and c = denom
   *
   * P1 register is an 18-bit value using following formula:
   *
   * 	P1[17:0] = 128 * a + floor(128*(b/c)) - 512
   *
   * P2 register is a 20-bit value using the following formula:
   *
   * 	P2[19:0] = 128 * b - c * floor(128*(b/c))
   *
   * P3 register is a 20-bit value using the following formula:
   *
   * 	P3[19:0] = c
   */

  /* Set the main PLL config registers */
  if (num == 0)
  {
    /* Integer mode */
    P1 = 128 * div - 512;
    P2 = num;
    P3 = denom;
  }
  else
  {
    /* Fractional mode */
    P1 = (uint32_t)(128 * div + floor(128 * ((float)num/(float)denom)) - 512);
    P2 = (uint32_t)(128 * num - denom * floor(128 * ((float)num/(float)denom)));
    P3 = denom;
  }

  /* Get the appropriate starting point for the PLL registers */
  uint8_t baseaddr = 0;
  switch (output)
  {
    case 0:
      baseaddr = SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1;
      break;
    case 1:
      baseaddr = SI5351_REGISTER_50_MULTISYNTH1_PARAMETERS_1;
      break;
    case 2:
      baseaddr = SI5351_REGISTER_58_MULTISYNTH2_PARAMETERS_1;
      break;
  }

  /* Set the MSx config registers */
  si5351_write8( baseaddr,   (P3 & 0x0000FF00) >> 8);
  si5351_write8( baseaddr+1, (P3 & 0x000000FF));
  si5351_write8( baseaddr+2, (P1 & 0x00030000) >> 16);	/* ToDo: Add DIVBY4 (>150MHz) and R0 support (<500kHz) later */
  si5351_write8( baseaddr+3, (P1 & 0x0000FF00) >> 8);
  si5351_write8( baseaddr+4, (P1 & 0x000000FF));
  si5351_write8( baseaddr+5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16) );
  si5351_write8( baseaddr+6, (P2 & 0x0000FF00) >> 8);
  si5351_write8( baseaddr+7, (P2 & 0x000000FF));


  if (pllSource == SI5351_PLL_A)
  {
          float fvco = m_si5351Config.plla_freq / (div + ( (float)num / (float)denom ));
          switch (output)
          {
           case 0:
           m_si5351Config.ms0_freq = (uint32_t)floor(fvco);
           break;
           case 1:
           m_si5351Config.ms1_freq = (uint32_t)floor(fvco);
           break;
           case 2:
           m_si5351Config.ms2_freq = (uint32_t)floor(fvco);
           break;
          }
  }
  else
  {
          float fvco = m_si5351Config.pllb_freq / (div + ( (float)num / (float)denom));
          switch (output)
          {
           case 0:
           m_si5351Config.ms0_freq = (uint32_t)floor(fvco);
           break;
           case 1:
           m_si5351Config.ms1_freq = (uint32_t)floor(fvco);
           break;
           case 2:
           m_si5351Config.ms2_freq = (uint32_t)floor(fvco);
           break;
          }
  }



  /* Configure the clk control and enable the output */
  uint8_t clkControlReg = 0x0F;                             /* 8mA drive strength, MS0 as CLK0 source, Clock not inverted, powered up */
  if (pllSource == SI5351_PLL_B) clkControlReg |= (1 << 5); /* Uses PLLB */
  if (num == 0) clkControlReg |= (1 << 6);                  /* Integer mode */
  switch (output)
  {
    case 0:
      ASSERT_STATUS(si5351_write8(SI5351_REGISTER_16_CLK0_CONTROL, clkControlReg));
      break;
    case 1:
      ASSERT_STATUS(si5351_write8(SI5351_REGISTER_17_CLK1_CONTROL, clkControlReg));
      break;
    case 2:
      ASSERT_STATUS(si5351_write8(SI5351_REGISTER_18_CLK2_CONTROL, clkControlReg));
      break;
  }

  return ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Enables or disables all clock outputs
*/
/**************************************************************************/
err_t si5351_enableOutputs(uint8_t enabled)
{
  /* Make sure we've called init first */
  ASSERT(m_si5351Config.initialised, ERROR_DEVICENOTINITIALISED);

  /* Enabled desired outputs (see Register 3) */
  ASSERT_STATUS(si5351_write8(SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, enabled ? 0x00: 0xFF));

  return ERROR_NONE;
}

/* ---------------------------------------------------------------------- */
/* PRUVATE FUNCTIONS                                                      */
/* ---------------------------------------------------------------------- */

/**************************************************************************/
/*!
    @brief  Writes a register and an 8 bit value over I2C
*/
/**************************************************************************/
err_t si5351_write8 (uint8_t reg, uint8_t value)
{
	HAL_StatusTypeDef status = HAL_OK;
  
	while (HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(SI5351_ADDRESS<<1), 3, 100) != HAL_OK) { }

    status = HAL_I2C_Mem_Write(&hi2c2,							// i2c handle
    						  (uint8_t)(SI5351_ADDRESS<<1),		// i2c address, left aligned
							  (uint8_t)reg,						// register address
							  I2C_MEMADD_SIZE_8BIT,				// si5351 uses 8bit register addresses
							  (uint8_t*)(&value),				// write returned data to this variable
							  1,								// how many bytes to expect returned
							  100);								// timeout

  return ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
err_t si5351_read8(uint8_t reg, uint8_t *value)
{
	HAL_StatusTypeDef status = HAL_OK;

	while (HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(SI5351_ADDRESS<<1), 3, 100) != HAL_OK) { }

    status = HAL_I2C_Mem_Read(&hi2c2,							// i2c handle
    						  (uint8_t)(SI5351_ADDRESS<<1),		// i2c address, left aligned
							  (uint8_t)reg,						// register address
							  I2C_MEMADD_SIZE_8BIT,				// si5351 uses 8bit register addresses
							  (uint8_t*)(&value),				// write returned data to this variable
							  1,								// how many bytes to expect returned
							  100);								// timeout

  return ERROR_NONE;
}

