#include "headers.h"

void spi_init()
{
    SPI_Initialize(&ADC,SPI0,0);
}


bool SPI_Transact_WithTimeout(SPI_Interface *spi,uint16_t *txData,uint16_t numTx,uint16_t *rxData,uint16_t numRx,uint32_t timeout_us)
{
     Delay_Timer_Start(&delay); 
    uint32_t startTime = Delay_Timer_Get(&delay);

    SPI_Transact(spi, txData, numTx, rxData, numRx);

    while (1)
    {
        if ((spi->rxState == SPI_IDLE) &&
            (spi->txState == SPI_IDLE))
        {
            break;
        }

        if ((Delay_Timer_Get(&delay) - startTime) >= timeout_us)
        {
            /* Reset SPI safely */
            DL_SPI_disable(spi->port);
            DL_SPI_enable(spi->port);

            spi->rxState = SPI_IDLE;
            spi->txState = SPI_IDLE;

            return false;
        }
    }

    return true;
}


uint16_t ADC122S625_Read(SPI_Interface *spi,uint8_t choice)
{

       uint16_t tx[2] = {0xFFFF,0XFFFF};
        uint16_t rx[2]={0XFFFF,0XFFFF};

    DL_GPIO_clearPins(Chip_select_PORT,Chip_select_Chip_select_bar_En_PIN);
    

    SPI_Transact(spi, tx, 2, rx, 2);

    
    delay_cycles(3200);
    DL_GPIO_setPins(Chip_select_PORT,Chip_select_Chip_select_bar_En_PIN);
    spi->rxState = SPI_IDLE;
    spi->txState = SPI_IDLE;
    if(rx[0]>0x7ff)
    {
    rx[0]=0xfff-rx[0];
    }
    if(rx[1]>0x7ff)
    {
    rx[1]=0xfff-rx[1];
    }
     return rx[choice];
}