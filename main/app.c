#include "headers.h"


 void UART0_IRQHandler(void)
{
    UART_Interface_IRQ(&USB_UART);
}

void TIMA0_IRQHandler(void)
{
    Timer_Timeout_IRQ(&USB_Timeout);
}