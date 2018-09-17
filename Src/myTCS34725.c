#include "myTCS34725.h"
void begin(void);
void setInterrupt(uint8_t i) ;
/*========================================================================*/
/*                          PRIVATE FUNCTIONS                             */
/*========================================================================*/
  uint8_t _tcs34725Initialised;
  tcs34725Gain_t _tcs34725Gain;
  tcs34725IntegrationTime_t _tcs34725IntegrationTime; 
/**************************************************************************/
/*!
    @brief  Implements missing powf function
*/
/**************************************************************************/
float powf(const float x, const float y)
{
  return (float)(pow((double)x, (double)y));
}

/**************************************************************************/
/*!
    @brief  Writes a register and an 8 bit value over I2C
*/
/**************************************************************************/
void write8 (uint8_t reg, uint8_t value)
{
  while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY);
  
  uint8_t pData[2];
  pData[0] = TCS34725_COMMAND_BIT | reg;
  pData[1] = value;
  
  HAL_I2C_Master_Transmit (&hi2c2, TCS34725_ADDRESS, pData, 2, 1000);
  
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t read8(uint8_t reg)
{
  while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY);
  
  uint8_t pData[1];
  reg |= TCS34725_COMMAND_BIT;
  HAL_I2C_Master_Transmit (&hi2c2, TCS34725_ADDRESS, &reg, 1, 1000);
  
  while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY);
  
  HAL_I2C_Master_Receive(&hi2c2, TCS34725_ADDRESS, pData, 1, 1000);
  
  
  return *pData;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit values over I2C
*/
/**************************************************************************/
void read16(uint8_t reg, uint16_t* pResult)
{
  while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY);
  
  uint8_t pData[2];
  reg |= TCS34725_COMMAND_BIT;
  HAL_I2C_Master_Transmit(&hi2c2, TCS34725_ADDRESS, &reg, 1, 1000);
  
  
  while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY);
  
  HAL_I2C_Master_Receive(&hi2c2, TCS34725_ADDRESS, pData, 2, 1000);
  
  //HAL_I2C_Mem_Read(&hi2c2, TCS34725_ADDRESS, TCS34725_COMMAND_BIT | reg, 1, pData, sizeof(pData), 1000);
  *pResult = (uint16_t)pData[1] << 8 | (uint16_t)pData[0];
}

/**************************************************************************/
/*!
    Enables the device
*/
/**************************************************************************/
void enable(void)
{
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  HAL_Delay(10);
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);  

}




void setInterrupt(uint8_t i) {
  uint8_t r = read8(TCS34725_ENABLE);
  if (i) {
    r |= TCS34725_ENABLE_AIEN;
  } else {
    r &= ~TCS34725_ENABLE_AIEN;
  }
  write8(TCS34725_ENABLE, r);
}

void clearInterrupt(void) {
  uint8_t pData[] = {0x66};
  
  HAL_I2C_Master_Transmit(&hi2c2, TCS34725_ADDRESS, pData, sizeof(pData), 1000);
}


void setIntLimits(uint16_t low, uint16_t high) {
   write8(0x04, low & 0xFF);
   write8(0x05, low >> 8);
   write8(0x06, high & 0xFF);
   write8(0x07, high >> 8);
}

/**************************************************************************/
/*!
    Sets the integration time for the TC34725
*/
/**************************************************************************/
void setIntegrationTime(tcs34725IntegrationTime_t it)
{
  if (!_tcs34725Initialised) begin();

  /* Update the timing register */
  write8(TCS34725_ATIME, it);

  /* Update value placeholders */
  _tcs34725IntegrationTime = it;
}

/**************************************************************************/
/*!
    Adjusts the gain on the TCS34725 (adjusts the sensitivity to light)
*/
/**************************************************************************/
void setGain(tcs34725Gain_t gain)
{
  if (!_tcs34725Initialised) begin();

  /* Update the timing register */
  write8(TCS34725_CONTROL, gain);

  /* Update value placeholders */
  _tcs34725Gain = gain;
}

/**************************************************************************/
/*!
    Initializes I2C and configures the sensor (call this function before
    doing anything else)
*/
/**************************************************************************/
void begin(void) 
{
  
  _tcs34725Initialised = 1;
  /* Set default integration time and gain */
  setIntegrationTime(_tcs34725IntegrationTime);
  setGain(_tcs34725Gain);
  
  /* Note: by default, the device is in power down mode on bootup */
  enable();

}
/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
void tsc_init(tcs34725IntegrationTime_t  it, tcs34725Gain_t gain) 
{
  _tcs34725Initialised = 0;
  _tcs34725IntegrationTime = it;
  _tcs34725Gain = gain;
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/


  

/**************************************************************************/
/*!
    @brief  Reads the raw red, green, blue and clear channel values
*/
/**************************************************************************/
void tsc_getRawData (uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
  if (!_tcs34725Initialised) begin();

  read16(TCS34725_CDATAL, c);
  read16(TCS34725_RDATAL, r);
  read16(TCS34725_GDATAL, g);
  read16(TCS34725_BDATAL, b);
  
  /* Set a delay for the integration time */
  switch (_tcs34725IntegrationTime)
  {
    case TCS34725_INTEGRATIONTIME_2_4MS:
      HAL_Delay(3);
      break;
    case TCS34725_INTEGRATIONTIME_24MS:
      HAL_Delay(24);
      break;
    case TCS34725_INTEGRATIONTIME_50MS:
      HAL_Delay(50);
      break;
    case TCS34725_INTEGRATIONTIME_101MS:
      HAL_Delay(101);
      break;
    case TCS34725_INTEGRATIONTIME_154MS:
      HAL_Delay(154);
      break;
    case TCS34725_INTEGRATIONTIME_700MS:
      HAL_Delay(700);
      break;
  }

}

/**************************************************************************/
/*!
    @brief  Converts the raw R/G/B values to color temperature in degrees
            Kelvin
*/
/**************************************************************************/
uint16_t tsc_calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b)
{
  float X, Y, Z;      /* RGB to XYZ correlation      */
  float xc, yc;       /* Chromaticity co-ordinates   */
  float n;            /* McCamy's formula            */
  float cct;

  /* 1. Map RGB values to their XYZ counterparts.    */
  /* Based on 6500K fluorescent, 3000K fluorescent   */
  /* and 60W incandescent values for a wide range.   */
  /* Note: Y = Illuminance or lux                    */
  X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
  Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
  Z = (-0.68202F * r) + (0.77073F * g) + ( 0.56332F * b);

  /* 2. Calculate the chromaticity co-ordinates      */
  xc = (X) / (X + Y + Z);
  yc = (Y) / (X + Y + Z);

  /* 3. Use McCamy's formula to determine the CCT    */
  n = (xc - 0.3320F) / (0.1858F - yc);

  /* Calculate the final CCT */
  cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

  /* Return the results in degrees Kelvin */
  return (uint16_t)cct;
}

/**************************************************************************/
/*!
    @brief  Converts the raw R/G/B values to color temperature in degrees
            Kelvin
*/
/**************************************************************************/
uint16_t tsc_calculateLux(uint16_t r, uint16_t g, uint16_t b)
{
  float illuminance;

  /* This only uses RGB ... how can we integrate clear or calculate lux */
  /* based exclusively on clear since this might be more reliable?      */
  illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

  return (uint16_t)illuminance;
}



/**************************************************************************/
/*!
    Disables the device (putting it in lower power sleep mode)
*/
/**************************************************************************/
void tsc_disable(void)
{
  /* Turn the device off to save power */
  _tcs34725Initialised = 0;
  uint8_t reg = 0;
  reg = read8(TCS34725_ENABLE);
  write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));  
  

}

void tsc_setIntgrTime(uint8_t t){
  switch (t){
    case 0:
      _tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_2_4MS;
    break;
    case 1:
      _tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_24MS;
    break;
    case 2:
      _tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_50MS;
    break;
    case 3:
      _tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_101MS;
    break;
    case 4:
      _tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_154MS;
    break;
    case 5:
      _tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_700MS;
    break;
    default:
      _tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_24MS;
    break;
  }
  
  setIntegrationTime(_tcs34725IntegrationTime);
}

uint8_t tsc_getIntgrTime(void){
  uint8_t r;
  switch (_tcs34725IntegrationTime){
    case TCS34725_INTEGRATIONTIME_2_4MS:
      r = 0;
    break;
    case TCS34725_INTEGRATIONTIME_24MS:
      r = 1;
    break;
    case TCS34725_INTEGRATIONTIME_50MS:
      r = 2;
    break;
    case TCS34725_INTEGRATIONTIME_101MS:
      r = 3;
    break;
    case TCS34725_INTEGRATIONTIME_154MS:
      r = 4;
    break;
    case TCS34725_INTEGRATIONTIME_700MS:
      r = 5;
    break;
    default:
      r = 1;
    break;
  }
  return r;
}



