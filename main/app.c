#include "headers.h"

void app_init(void) {
  usb_init();
  cmd_data_setup();
  i2c_init();
  mux_init_all();
  control_init();
}

void cmd_data_setup(void)
{
    
}

void time_cal_init()
{
     Delay_Timer_Initialize(&delay,TIMG7,7,32000000);  
}

void UART0_IRQHandler(void) { UART_Interface_IRQ(&USB_UART); }

void TIMA0_IRQHandler(void) { Timer_Timeout_IRQ(&USB_Timeout); }

void I2C1_IRQHandler(void) { I2C_Controller_IRQ(&I2C_MCU); }

void TIMG7_IRQHandler(void) { Delay_Timer_IRQ(&delay); }

void SPI0_IRQHandler(void) { SPI_IRQ(&ADC); }
