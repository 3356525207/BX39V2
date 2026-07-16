/**************************************************
** Copyright (c) 2016-202X 昆泰芯微电子科技有限公司
** 文件名: kth78xx.c
** 作者: liujunbo
** 日期: 2023.08.16
** 描述: KTH78xx芯片相关文件，存放用来操作KTH78xx芯片的函数
**
**************************************************/

#include "kth78xx.h"
#include "at32m412_416_wk_config.h"



static uint8_t KTH78_SPI_TransmitReceiveBytes( uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
  SPI2_CS_LOW();
	
// while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET); 
//spi_i2s_data_transmit(SPI2, pTxData[0]);
// while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
//spi_i2s_data_transmit(SPI2, pTxData[1]);

//	
// while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET); 
//pRxData[0]=spi_i2s_data_receive(SPI2);
// while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET); 
//pRxData[1]=spi_i2s_data_receive(SPI2);
//while(spi_i2s_flag_get(SPI2, SPI_I2S_BF_FLAG) != RESET); 
	

	
	SPI2_CS_HIGH();
	

  return 0;
}



//读取角度值
uint16_t KTH78_ReadAngle(void)
{
  uint16_t databack;
  SPI2_CS_LOW();
	
 while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET); 
spi_i2s_data_transmit(SPI2, 0x0000);

 while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET); 
databack=spi_i2s_data_receive(SPI2);

while(spi_i2s_flag_get(SPI2, SPI_I2S_BF_FLAG) != RESET); 
	
	SPI2_CS_HIGH();

  return (uint16_t)databack;
}

//读取寄存器数据
uint8_t KTH78_ReadReg(uint8_t addr)
{
  uint8_t databack[2];
  uint8_t cmd[2];

  cmd[0] = KTH78_READ_REG + addr;
  cmd[1] = KTH78_NON_DATA;

  KTH78_SPI_TransmitReceiveBytes( cmd, databack, 2);

  return databack[0];
}

//写寄存器数据
uint8_t KTH78_WriteReg(uint8_t addr, uint8_t data)
{
  uint8_t databack[2];
  uint8_t cmd[2];

  cmd[0] = KTH78_WRITE_REG + addr;
  cmd[1] = data;

  KTH78_SPI_TransmitReceiveBytes( cmd, databack, 2);

  return databack[0];
}

//向MTP中写入配置
uint8_t KTH78_WriteMTP(uint8_t addr, uint8_t data)
{
  uint8_t databack[2];
  uint8_t cmd[2];

  cmd[0] = KTH78_WRITE_MTP + addr;
  cmd[1] = data;

  KTH78_SPI_TransmitReceiveBytes( cmd, databack, 2);

  return databack[0];
}
