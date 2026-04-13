#ifndef USB_H_
#define USB_H_

#include <stdint.h>
#include <string.h>

#define MAX_NUM_CMDS   36
#define MAX_CMD_LEN    64
#define MAX_RESP_LEN   64

typedef void (*process_cmd_cb_t)(void);



typedef enum{
    ERROR_NONE = 0,
    INVALID_COMMAND
} USB_errors_t;



typedef struct{
    char command[MAX_CMD_LEN];
    uint8_t cmd_len;
    uint8_t response[MAX_RESP_LEN];
    uint8_t resp_len;
    process_cmd_cb_t actions_func;
} USB_command_map_t;



typedef struct{
    USB_command_map_t command_table[MAX_NUM_CMDS];

    uint8_t rx_byte;                     //  single byte receive
    uint8_t cmd_buffer[MAX_CMD_LEN];

    uint8_t cmd_index;
    uint8_t cmd_len;

    uint8_t uartTargetIndex;

} UART_DATA;



void usb_init(void);
void usb_fsm(UART_DATA *param);

USB_errors_t validate_usb_rx_data(UART_DATA *usb_data);
void process_usb_command(UART_DATA *usb_data);

void usb_cmd_data_setup(UART_DATA *param,uint8_t index, const char *cmd,const char *response,process_cmd_cb_t cb_fn);



#endif