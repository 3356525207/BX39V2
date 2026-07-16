#ifndef __HKT7823_H__
#define __HKT7823_H__

#include "at32m412_416_conf.h"

#define SPI2_CS_HIGH()   gpio_bits_set(GPIOF, GPIO_PINS_12) 
#define SPI2_CS_LOW()    gpio_bits_reset(GPIOF, GPIO_PINS_12) 

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#endif






