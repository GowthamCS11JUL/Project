#include "command_function.h"
#include "adc_mux.h"
#include "headers.h"
#include "i2c_common.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "variables.h"

/* =======================
   Slave Addresses
======================= */
#define SLAVE_U6_0 0x4A
#define SLAVE_U7_0 0x40
#define SLAVE_U15  0x3B
#define MAX_VOLTAGE_MV  5000
#define PREFIX        "SET_VOLTAGE_"
#define SET_VOLTAGE_PREFIX_LEN 12


static uint32_t return_data = 0;

/* =======================
   HELPER FUNCTIONS
======================= */

//  Manual HEX conversion 
static void send_u32_hex(uint32_t value)
{
    const char hex[] = "0123456789ABCDEF";
    char buffer[8];

    for (int i = 0; i < 8; i++)
    {
        buffer[7 - i] = hex[value & 0xF];
        value >>= 4;
    }

    memcpy(executing_command->response, buffer, 8);
    executing_command->resp_len = 8;
}

// 🔴 FAIL response
static void send_fail(void)
{
    memcpy(executing_command->response, "FAIL", 4);
    executing_command->resp_len = 4;
}

/* =======================
   DATA TRANSMIT
======================= */

static void data_transmit(uint32_t status)
{
    send_u32_hex(status);
}

/* =======================
   RESET FUNCTIONS
======================= */

void reset_i2c(void)
{
    reset_current_measurement();
    reset_io_expander();
    data_transmit(0X00);
}

void reset_io_expander(void)
{
    uint8_t write_cmd[2] = {0x01, 0x00};
    uint8_t read_cmd[1]  = {0x01};

    io_expander_config(SLAVE_U15);
    i2c_common(SLAVE_U15, write_cmd, 2, read_cmd, 1);
}

void reset_current_measurement(void)
{
    voltage_current_measurement_reset_site_0();
}

void reset_dac(void)
{
    uint8_t write_cmd[3] = {0x08, 0x00, 0x10};
    uint8_t read_cmd[1]  = {0x08};
    i2c_common(SLAVE_U6_0, write_cmd, 3, read_cmd, 2);
}

/* =======================
   MUX + ADC
======================= */

static void read_mux_adc(uint8_t mux1, uint8_t mux2,
                         uint8_t mux3, uint8_t mux4,
                         uint8_t mux_on)
{
    uint32_t out = 0;

    if (mux_on & 0x01)
    {
        /* Step 1: Disable all muxes */
        mux_disable_all();

        /* Step 2: Select both mux channels */
        mux_select(MUX_ID_1, mux1);  // enables MUX1
        mux_select(MUX_ID_2, mux2);  // enables MUX2

        /* Step 3: stabilization */
        delay_cycles(64);

        /* Step 4: dummy read */
        ADC122S625_Read(&ADC, 0);
        delay_cycles(64);

        /* Step 5: actual read */
        out = ADC122S625_Read(&ADC, 0);

        send_u32_hex(out);
        return;
    }

    send_fail();
}
/* =======================
   IO EXPANDER
======================= */

void io_expander_config(uint8_t slave_address)
{
    uint8_t write_cmd[2] = {0x03, 0x00};
    uint8_t read_cmd[1]  = {0x00};

    i2c_common(slave_address, write_cmd, 2, read_cmd, 1);
}

/* =======================
   OPAMP
======================= */

void opamp_en(uint8_t slave_address)
{

    uint8_t opamp_select =0X10;

    io_expander_config(slave_address);

    uint8_t write_cmd[2] = {0x01, opamp_select};
    uint8_t read_cmd[1]  = {0x00};

    i2c_common(slave_address, write_cmd, 2, read_cmd, 1);
}

/* =======================
   POWER CONTROL
======================= */

void ldo_power_off_site_0(void)
{
    uint8_t write_cmd[2] = {0x01, 0x00};
    uint8_t read_cmd[1]  = {0x01};

    io_expander_config(SLAVE_U15);
    i2c_common(SLAVE_U15, write_cmd, 2, read_cmd, 1);
}

void ldo_power_on_site_0(uint8_t value)
{
    value |= 0x04;

    uint8_t write_cmd[2] = {0x01, value};
    uint8_t read_cmd[1]  = {0x01};

    io_expander_config(SLAVE_U15);

    return_data = i2c_common(SLAVE_U15, write_cmd, 2, read_cmd, 1);
    data_transmit(return_data);
}

void power_supply_3v3_1_site_0(void)
{
    ldo_power_off_site_0();

    uint8_t write_cmd[3] = {0x08, 0xA8, 0xF5};
    uint8_t read_cmd[1]  = {0x08};

    return_data = i2c_common(SLAVE_U6_0, write_cmd, 3, read_cmd, 2);

    opamp_en(SLAVE_U15);

    data_transmit(return_data);
}


void power_supply_3v3_2_site_0(void)
{
    ldo_power_off_site_0();

    uint8_t write_cmd[2] = {0x01, 0x08};
    uint8_t read_cmd[1]  = {0x01};

    io_expander_config(SLAVE_U15);
    i2c_common(SLAVE_U15, write_cmd, 2, read_cmd, 1);

    ldo_power_on_site_0(write_cmd[1]);
}

void power_supply_2v7_1_site_0(void)
{
    // ldo_power_off_site_0();

    uint8_t write_cmd[3] = {0x08, 0x8A, 0x3D};
    uint8_t read_cmd[1]  = {0x08};

    return_data = i2c_common(SLAVE_U6_0, write_cmd, 3, read_cmd, 2);

    opamp_en(SLAVE_U15);

    data_transmit(return_data);
}

void power_supply_2v7_2_site_0(void)
{
    // ldo_power_off_site_0();

    uint8_t write_cmd[2] = {0x01, 0x0D};
    uint8_t read_cmd[1]  = {0x01};

    io_expander_config(SLAVE_U15);
    return_data =i2c_common(SLAVE_U15, write_cmd, 2, read_cmd, 1);
    data_transmit(return_data);

    // ldo_power_on_site_0(write_cmd[1]);
}

/* =======================
   MEASUREMENTS
======================= */

void battery_voltage_check_site_0(void)
{
    read_mux_adc(0x00, 0x00, 0x00, 0x00, 0x01);
}

void switch_voltage_default_site_0(void)
{
    read_mux_adc(0x01, 0x00, 0x00, 0x00, 0x01);
}

void led_status_voltage_0_0_site_0(void)
{
    read_mux_adc(0x02, 0x00, 0x00, 0x00, 0x01);
}

void led_status_voltage_0_1_site_0(void)
{
    read_mux_adc(0x03, 0x00, 0x00, 0x00, 0x01);
}

void led_status_voltage_1_0_site_0(void)
{
    read_mux_adc(0x04, 0x00, 0x00, 0x00, 0x01);
}

void led_status_voltage_2_0_site_0(void)
{
    read_mux_adc(0x05, 0x00, 0x00, 0x00, 0x01);
}

void led_status_voltage_1_site_0(void)
{
    read_mux_adc(0x06, 0x06, 0x00, 0x00, 0x01);
}

void led_status_voltage_2_site_0(void)
{
    read_mux_adc(0x07, 0x07, 0x00, 0x00, 0x01);
}

void led_status_voltage_3_site_0(void)
{
    read_mux_adc(0x08, 0x08, 0x00, 0x00, 0x01);
}

void differentail_voltage_ic_led_0(void)
{
    read_mux_adc(0x0A, 0x00,0x00,0x00, 0x01);
}

void differentail_voltage_ic_led_1(void)
{
   read_mux_adc(0x0B, 0x00,0x00,0x00, 0x01); 
}

void differentail_voltage_ic_led_2(void)
{
   read_mux_adc(0x0C, 0x00,0x00,0x00, 0x01); 
}

void differentail_voltage_init_led_0(void)
{
   read_mux_adc(0x00, 0x0B,0x00,0x00, 0x01); 
}

void differentail_voltage_init_led_1(void)
{
read_mux_adc(0x00, 0x0C,0x00,0x00, 0x01); 
}
void differentail_voltage_init_led_2(void)
{
read_mux_adc(0x00, 0x0D,0x00,0x00, 0x01); 
}
void switch_voltage_press_site_0(void)
{
    switch_on();
    // read_mux_adc(0x01, 0x00, 0x00, 0x00, 0x01);
    Delay_Timer_Start(&delay);
    uint32_t start = Delay_Timer_Get(&delay);
    while ((Delay_Timer_Get(&delay) - start) < 210000);
    read_mux_adc(0x01, 0x00, 0x00, 0x00, 0x01);
    switch_off();

   
}

    
void voltage_current_measurement_site_on(void)
{
    // uint8_t write_cmd_1[3] = {0x00, 0x00, 0x10};  // CONFIG (ADCRANGE=0)
    // uint8_t write_cmd_2[3] = {0x02, 0x11, 0x90};  // SHUNT_CAL (correct!)

    uint8_t read_cmd[3] = {0x04, 0x00, 0x00};     // CURRENT register

    uint32_t value = 0;

    /* Configure INA228 */
    // i2c_common(SLAVE_U7_0, write_cmd_1, 3, NULL, 0);
    // i2c_common(SLAVE_U7_0, write_cmd_2, 3, NULL, 0);

    /* Read current */
    value = i2c_common(SLAVE_U7_0, NULL, 0, read_cmd, 3);
    value=value>>4;
    /* Sign extend 20-bit */
   if(value>0x7FFFF)
   {
   value=0XFFFFF-value;
   }


    /* Send raw or converted */
    data_transmit(value);
}


void voltage_current_measurement_reset_site_0(void)
{
    uint8_t write_cmd[3] = {0x00, 0x80,0X00};
    uint8_t read_cmd[1]  = {0x00};

    i2c_common(SLAVE_U7_0, write_cmd, 3, read_cmd, 0);
}

void turn_off_all(void)
{
    reset_i2c();
}

void apply_voltage(uint16_t mv)
{
    /* Example: 0–5V → 12-bit DAC */
    uint16_t dac = (mv * 4095U) / 5000U;
    
    ldo_power_off_site_0();
    uint16_t msb_byte=(dac&0xFF00)>>8;
    uint16_t lsb_byte=((dac)&0x00FF);
    uint8_t write_cmd[3] = {0x08, msb_byte, lsb_byte};
    uint8_t read_cmd[1]  = {0x08};

    return_data = i2c_common(SLAVE_U6_0, write_cmd, 3, read_cmd, 2);

    opamp_en(SLAVE_U15);

    data_transmit(return_data);

}

// void set_voltage_handler(void)
// {
//     char *cmd = (char*)USB_DATA.cmd_buffer;

//     /* Move pointer after "SET_VOLTAGE_" */
//     char *arg = cmd + strlen("SET_VOLTAGE_");

//     uint16_t voltage = 0;

//     /* ---- Manual parsing (embedded safe) ---- */
//     if (*arg == '\0')
//     {
//         /* No value */
//         const char *err = "ERR";
//         memcpy(executing_command->response, err, 3);
//         executing_command->resp_len = 3;
//         return;
//     }

//     while (*arg)
//     {
//         if (*arg < '0' || *arg > '9')
//         {
//             const char *err = "ERR";
//             memcpy(executing_command->response, err, 3);
//             executing_command->resp_len = 3;
//             return;
//         }

//         voltage = voltage * 10 + (*arg - '0');
//         arg++;
//     }

//     /* ---- Range check ---- */
//     if (voltage > MAX_VOLTAGE_MV)
//     {
//         const char *err = "RANGE_ERROR";
//         memcpy(executing_command->response, err, 5);
//         executing_command->resp_len = 5;
//         return;
//     }

//     /* ---- Apply voltage ---- */
//     apply_voltage(voltage);

//     /* ---- Response ---- */
//     const char *ok = "PASS";
//     memcpy(executing_command->response, ok, 4);
//     executing_command->resp_len = 4;
// }

// void set_voltage_handler(void)
// {
//     char *cmd = (char*)USB_DATA.cmd_buffer;

//     /* 🔥 Ensure correct command prefix */
//     if (strncmp(cmd, PREFIX, PREFIX_LEN) != 0)
//     {
//         const char *err = "INVALID";
//         memcpy(executing_command->response, err, 7);
//         executing_command->resp_len = 7;
//         return;
//     }

//     /* Move pointer after prefix */
//     char *arg = cmd + PREFIX_LEN;

//     uint16_t voltage = 0;

//     /* ---- Check empty ---- */
//     if (*arg == '\0')
//     {
//         const char *err = "ERR";
//         memcpy(executing_command->response, err, 3);
//         executing_command->resp_len = 3;
//         return;
//     }

//     /* ---- Parse number safely ---- */
//     while (*arg)
//     {
//         if (*arg < '0' || *arg > '9')
//         {
//             const char *err = "ERR";
//             memcpy(executing_command->response, err, 3);
//             executing_command->resp_len = 3;
//             return;
//         }

//         /* Prevent overflow */
//         if (voltage > 5000)
//         {
//             const char *err = "RANGE_ERROR";
//             memcpy(executing_command->response, err, 11);
//             executing_command->resp_len = 11;
//             return;
//         }

//         voltage = voltage * 10 + (*arg - '0');
//         arg++;
//     }

//     /* ---- Final range check ---- */
//     if (voltage > MAX_VOLTAGE_MV)
//     {
//         const char *err = "RANGE_ERROR";
//         memcpy(executing_command->response, err, 11);
//         executing_command->resp_len = 11;
//         return;
//     }

//     /* ---- Apply voltage ---- */
//     apply_voltage(voltage);

//     /* ---- Response ---- */
//     const char *ok = "PASS";
//     memcpy(executing_command->response, ok, 4);
//     executing_command->resp_len = 4;
// }



void set_voltage_handler(void)
{
    // Points to the data immediately after "SET_VOLTAGE_"
    char *arg = (char *)(USB_DATA.cmd_buffer + SET_VOLTAGE_PREFIX_LEN);
    uint32_t voltage = 0;

    // 1. Check if argument exists
    if (*arg == '\0') {
        set_usb_response("ERR_MISSING_VALUE");
        return;
    }

    // 2. Parse the numeric string
    while (*arg >= '0' && *arg <= '9') {
        voltage = (voltage * 10) + (*arg - '0');
        arg++;
        
        // Safety break to prevent massive number overflow
        if (voltage > 10000) break; 
    }

    // 3. Validation
    if (voltage > 5000) {
        set_usb_response("ERR_OUT_OF_RANGE"); // Voltage too high
        return;
    }

    // 4. Action
    apply_voltage((uint16_t)voltage);
    
    // 5. Response
    set_usb_response("PASS");
}

/**
 * Helper to update the current command's response and length safely.
 */
void set_usb_response(const char *msg)
{
    if (executing_command != NULL && msg != NULL)
    {
        uint8_t len = strlen(msg);
        if (len > MAX_RESP_LEN) len = MAX_RESP_LEN;
        
        memcpy(executing_command->response, msg, len);
        executing_command->resp_len = len;
    }
}