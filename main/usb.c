#include "headers.h"
#include "usb.h"
 

 
void usb_cmd_data_setup(UART_DATA *param,uint8_t index,const char *cmd,const char *response,process_cmd_cb_t cb_fn)
{
    if(index >= MAX_NUM_CMDS)
        return;
 
    uint8_t len = strlen(cmd);
 
    memcpy(param->command_table[index].command, cmd, len);
    param->command_table[index].command[len] = '\0';
    param->command_table[index].cmd_len = len;
 
    if(response != NULL)
    {
        uint8_t resp_len = strlen(response);
        memcpy(param->command_table[index].response, response, resp_len);
        param->command_table[index].resp_len = resp_len;
    }
    else
    {
        param->command_table[index].resp_len = 0;
    }
 
    param->command_table[index].actions_func = cb_fn;
}
 
 
 
void usb_init(void)
{
    memset(&USB_DATA, 0, sizeof(USB_DATA));
 
    UART_Interface_Initialize(&USB_UART, UART0, 0, 255, 255);
    Timer_Timeout_Inialize(&USB_Timeout, TIMA0, 0, CPUCLK_FREQ);
    UART_Target_Initialize(&USB_UART, 0, &USB_Timeout, 0, 100000);
}
 
 
 
 
void usb_fsm(UART_DATA *param)
{
    UART_Target_State state;
 
    state = UART_Target_getState(param->uartTargetIndex);
 
    switch (state)
    {
        case UART_TARGET_IDLE:
            /* Receive 1 byte only (no blocking, no timeout delay) */
            UART_Target_Receive(param->uartTargetIndex,&param->rx_byte, 1);
            break;
 
        case UART_TARGET_RECEIVED:
        {
            uint8_t byte = param->rx_byte;
 
            /* End of command */
            if(byte == '\r' || byte == '\n' || byte=='\0')
            {
            if(byte == '\r' || byte == '\n')
            {
                if(param->cmd_index > 0)
                {
                    param->cmd_buffer[param->cmd_index] = '\0';
                    param->cmd_len = param->cmd_index;
 
                    process_usb_command(param);
                }
 
                param->cmd_index = 0;
            }
            // else if(byte=='\0' && param->cmd_buffer[param->cmd_index++]=='\0' )
            // {
            //      process_usb_command(param);
            //     param->cmd_index = 0;
            // }
            }
            else
            {
                if(param->cmd_index < MAX_CMD_LEN - 1)
                {
                    param->cmd_buffer[param->cmd_index++] = byte;
                }
                else
                {
                    /* Overflow protection */
                    param->cmd_index = 0;
                }
            
            }
            
 
            /* Immediately re-arm receive */
            UART_Target_Receive(param->uartTargetIndex,
                                &param->rx_byte,
                                1);
            break;
        }
 
        default:
            break;
    }
}
 
 
 
USB_errors_t validate_usb_rx_data(UART_DATA *usb_data)
{
    for (uint8_t i = 0; i < MAX_NUM_CMDS; i++)
    {
        if (usb_data->cmd_len ==
            usb_data->command_table[i].cmd_len &&
            memcmp(usb_data->cmd_buffer,
                   usb_data->command_table[i].command,
                   usb_data->cmd_len) == 0)
        {
            executing_command = &usb_data->command_table[i];
            return ERROR_NONE;
        }
    }
 
    return INVALID_COMMAND;
}
 
/* ------------------------------------------------ */
/* PROCESS COMMAND                                  */
/* ------------------------------------------------ */
 
void process_usb_command(UART_DATA *usb_data)
{
    static char err_response[] = "INVALID_COMMAND";
 
    if (validate_usb_rx_data(usb_data) == ERROR_NONE)
    {
        if(executing_command->actions_func != NULL)
            executing_command->actions_func();
 
        if(executing_command->resp_len > 0)
        {
            UART_Target_Transmit(usb_data->uartTargetIndex,
                                 executing_command->response,
                                 executing_command->resp_len);
        }
    }
    else
    {
        UART_Target_Transmit(usb_data->uartTargetIndex,
                             (uint8_t *)err_response,
                             strlen(err_response));
    }
}