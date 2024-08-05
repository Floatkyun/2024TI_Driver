#include "PE43xx.H"
 
 
 uint8_t _type=02; // PE4302 : 02
float _max;
float _step;
float _level;
 
//type写PE43xx (PE4302/PE4306/PE4312)
 
void PE43xx_f( uint8_t type) {
//	_pin_le = pin_le;
//	_pin_clock = pin_clock;
//	_pin_data = pin_data;
	_type = type;
}

void begin() {
    // set our internal max and step values
			GPIO_InitTypeDef GPIO_InitStruct = {0};
			
			__HAL_RCC_GPIOB_CLK_ENABLE();
			
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;


	GPIO_InitStruct.Pin = _pin_le_Pin;
	HAL_GPIO_Init(_pin_le_GPIO_Port, &GPIO_InitStruct);
			
	GPIO_InitStruct.Pin = _pin_clock_Pin;
	HAL_GPIO_Init(_pin_clock_GPIO_Port, &GPIO_InitStruct);	

	GPIO_InitStruct.Pin = _pin_data_Pin;
	HAL_GPIO_Init(_pin_data_GPIO_Port, &GPIO_InitStruct);		

			
    switch (_type) {
        case PE4302:
        case PE4312:
            _max = 31.5;
            _step = 0.5;
            break;
        case PE4306:
            _max = 30;
            _step = 1;
            break;
        default:
            #warning type not certain, going with defaults, step=1 max=30
            _step = 1;
            _max = 30;
            break;
    }
 
    // set level to 0
    //setLevel(0);
}
 
int setLevel(float level) {
    // bounds check
    if(level < 0 || level > getMax() ) {
        return 0;
    }
 
    _level = level;
    _writeLevel();
 
    return 1;
}
 
void _writeLevel() {
 
    // PE4302 and PE4312 wants something like this
    //
    // C16  C8  C4  C2  C1  C0.5  =>  result
    //  0    0   0   0   0    0   =>  0db
    //  0    0   0   0   0    1   =>  0.5db
    //  0    0   1   1   0    1   =>  6.5db
    //  1    1   1   1   1    1   => 31.5db
 
    // for PE4306 (which has only 1db resolution)
    //
    // C16  C8  C4  C2  C1  =>  result
    //  0    0   0   0   0  =>  0db
    //  0    0   0   0   1  =>  1db
    //  0    1   0   0   1  =>  9db
    //  1    1   1   1   1  => 31db
 
    // so the big different is that one can set the .5 bit or not
 
    // get the integer part, it will be the same for both chips
    int intlevel = _level;//_level==31.5...,int intlevel==31
    // default to 5 bits
    int bits = 5;
 
    // if our step is less than 1 the chip supports the 0.5 decimal
    if(_step < 1) {
        // one more bit
        bits = 6;
 
        // if the decimal is not 0 shift 1 into our level
        if( (_level*10 - intlevel*10) > 0) //有小数部分
        {
            intlevel = intlevel << 1;
//            bitWrite(intlevel, 0, 1);
            intlevel+=1;
        } else {
            intlevel = intlevel << 1;
//            bitWrite(intlevel, 0, 0);
 
        }
    }
 
    // LE and CLOCK down to get the chip listen
//    digitalWrite(_pin_le, LOW);
//    digitalWrite(_pin_clock,LOW);
    HAL_GPIO_WritePin(_pin_le_GPIO_Port, _pin_le_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_pin_clock_GPIO_Port, _pin_clock_Pin, GPIO_PIN_RESET);
    // Write the level out MSB first
    int b;
    for (int bit = bits; bit > 0; bit--) {
        b = ((intlevel << 1) >> bit) & 0x01;
 
        HAL_GPIO_WritePin(_pin_data_GPIO_Port, _pin_data_Pin, b);
        HAL_GPIO_WritePin(_pin_clock_GPIO_Port, _pin_clock_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(_pin_clock_GPIO_Port, _pin_clock_Pin, GPIO_PIN_RESET);
    }
 
    // toggle LE to latch
//    digitalWrite(_pin_le, HIGH); // Toggle LE high to enable latch
//    digitalWrite(_pin_le, LOW);  // and then low again to hold it.
        HAL_GPIO_WritePin(_pin_le_GPIO_Port, _pin_le_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(_pin_le_GPIO_Port, _pin_le_Pin, GPIO_PIN_RESET);
}
 
// getters
float getLevel() {
    return _level;
}
 
float getMax() {
    return _max;
}
 
float getStep() {
    return _step;
}
 
 
//main调用函数
void PE43xx_init()//第一次用
{
	//PE43xx_f(type);
	begin();
	//setLevel(level);
 
 
}
void PE43xx_changelevel(float level)//非第一次用
{
	setLevel(level);
 
}
 
 
 