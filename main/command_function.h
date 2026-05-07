#ifndef COMMAND_FUNCTION_H
#define COMMAND_FUNCTION_H

#include <stdint.h>

/* =========================================================
   RESET FUNCTIONS
   ========================================================= */

/**
 * @brief Reset I2C-related system
 */
void reset_i2c(void);

/**
 * @brief Reset IO expander
 */
void reset_io_expander(void);

/**
 * @brief Reset current measurement IC
 */
void reset_current_measurement(void);

/**
 * @brief Reset DAC
 */
void reset_dac(void);

/**
 * @brief Turn OFF entire system
 */
void turn_off_all(void);

/* =========================================================
   IO EXPANDER
   ========================================================= */

/**
 * @brief Configure IO expander
 * @param slave_address I2C address
 */
void io_expander_config(uint8_t slave_address);

/* =========================================================
   OPAMP CONTROL
   ========================================================= */

/**
 * @brief Enable opamp
 * @param slave_address I2C address
 */
void opamp_en(uint8_t slave_address);

/* =========================================================
   POWER CONTROL
   ========================================================= */

/**
 * @brief Turn OFF LDO (Site 0)
 */
void ldo_power_off_site_0(void);

/**
 * @brief Turn ON LDO (Site 0)
 * @param value configuration bits
 */
void ldo_power_on_site_0(uint8_t value);

/**
 * @brief 3.3V Power mode 1
 */
void power_supply_3v3_1_site_0(void);

/**
 * @brief 3.3V Power mode 2
 */
void power_supply_3v3_2_site_0(void);

/**
 * @brief 2.7V Power mode 1
 */
void power_supply_2v7_1_site_0(void);

/**
 * @brief 2.7V Power mode 2
 */
void power_supply_2v7_2_site_0(void);

/* =========================================================
   MEASUREMENTS (MUX + ADC)
   ========================================================= */

/**
 * @brief Battery voltage
 */
void battery_voltage_check_site_0(void);

/**
 * @brief Switch default voltage
 */
void switch_voltage_default_site_0(void);

/**
 * @brief LED voltage checks
 */
void led_status_voltage_0_0_site_0(void);
void led_status_voltage_0_1_site_0(void);
void led_status_voltage_1_0_site_0(void);
void led_status_voltage_2_0_site_0(void);

void led_status_voltage_1_site_0(void);
void led_status_voltage_2_site_0(void);
void led_status_voltage_3_site_0(void);

/**
 * @brief Differential voltage measurements
 */
void differentail_voltage_ic_led_0(void);
void differentail_voltage_ic_led_1(void);
void differentail_voltage_ic_led_2(void);

void differentail_voltage_init_led_0(void);
void differentail_voltage_init_led_1(void);
void differentail_voltage_init_led_2(void);

/**
 * @brief Switch press measurement
 */
void switch_voltage_press_site_0(void);

/* =========================================================
   CURRENT MEASUREMENT
   ========================================================= */

/**
 * @brief Read current (INA228)
 */
void voltage_current_measurement_site_on(void);

/**
 * @brief Reset current measurement
 */
void voltage_current_measurement_reset_site_0(void);

void set_voltage_handler(void);

void apply_voltage(uint16_t mv);
void set_usb_response(const char *msg);

#endif /* COMMAND_FUNCTION_H */