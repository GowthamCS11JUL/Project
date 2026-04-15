#include "headers.h"

/* =======================
   Buffers
======================= */
static uint8_t verify_data[16];
static uint8_t tx_buffer[16];

/* =======================
   I2C Initialized 
======================= */

void i2c_init(void)
{
   I2C_Controller_Initialize(&I2C_MCU, I2C1, 1);
}

/* =======================
   Timeout Handler
======================= */
static bool I2C_Wait_With_Timeout(void)
{
    uint32_t start = Delay_Timer_Get(&delay);
    uint32_t timeout_us = 500000;  // 500 ms

    while (!I2C_Controller_Transaction_Done(&I2C_MCU))
    {
        if ((Delay_Timer_Get(&delay) - start) >= timeout_us)
        {
            I2C_MCU.state = I2C_CONTROLLER_IDLE;
            return false;
        }
    }

    if (I2C_MCU.error != I2C_CONTROLLER_ERROR_NONE)
        return false;

    return true;
}

/* =======================
   I2C COMMON FUNCTION
======================= */
uint32_t i2c_common(uint8_t slave_address,uint8_t *write_command,uint8_t write_cmd_len,uint8_t *read_command,uint8_t read_cmd_len)
{
    uint32_t result = 0;

    /* -------- BUSY CHECK -------- */
    if (I2C_MCU.state != I2C_CONTROLLER_IDLE)
    {
        return 0xFFFFFFFF;  // BUSY ERROR
    }

    /* -------- WRITE -------- */
    if (write_cmd_len > 0 && write_command != NULL)
    {
        /* Safe copy */
        memcpy(tx_buffer, write_command, write_cmd_len);

        I2C_Controller_Start_Transmit(&I2C_MCU,slave_address,tx_buffer,write_cmd_len);

        if (!I2C_Wait_With_Timeout())
            return 0xEEEEEEEE; // TIMEOUT
    }

    /* -------- READ -------- */
    if (read_cmd_len > 0 && read_command != NULL)
    {
        uint8_t reg;

        /* Copy register safely */
        memcpy(&reg, read_command, 1);

        /* Set register pointer */
        I2C_Controller_Start_Transmit(&I2C_MCU,slave_address,&reg,1);

        if (!I2C_Wait_With_Timeout())
            return 0xEEEEEEEE;

        /* Read data */
        I2C_Controller_Start_Receive(&I2C_MCU,slave_address,verify_data,read_cmd_len);

        if (!I2C_Wait_With_Timeout())
            return 0xEEEEEEEE;
    }

    /* -------- WRITE + VERIFY -------- */
    if (write_cmd_len > 0 && read_cmd_len > 0)
    {
        /* Compare written data (skip register byte) */
        if (memcmp(&write_command[1], verify_data, read_cmd_len) == 0)
            return 1;   // SUCCESS
        else
            return 0;   // FAIL
    }

    /* -------- READ ONLY -------- */
    if (read_cmd_len > 0)
    {
        result = 0;

        for (uint8_t i = 0; i < read_cmd_len; i++)
        {
            result <<= 8;
            result |= verify_data[i];
        }

        return result;
    }

    /* -------- WRITE ONLY -------- */
    return 1;
}