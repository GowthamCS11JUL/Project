#ifndef VARIABLES_H_
#define VARIABLES_H_

UART_Interface USB_UART;
Timer_Timeout USB_Timeout;
UART_DATA USB_DATA;
USB_command_map_t *executing_command;
I2C_Controller I2C_MCU;
Delay_Timer delay;
SPI_Interface ADC;

#endif