#ifndef ADC_SPI_H_
#define ADC_SPI_H_

void spi_init(void);
bool SPI_Transact_WithTimeout(SPI_Interface *spi,uint16_t *txData,uint16_t numTx,uint16_t *rxData,uint16_t numRx,uint32_t timeout_us);
uint16_t ADC122S625_Read(SPI_Interface *spi,uint8_t choice);


#endif

