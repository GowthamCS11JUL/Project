#include "control.h"
#include "headers.h"
#include "ti/driverlib/dl_gpio.h"

void contorl_init(void) {
  DL_GPIO_initDigitalOutput(RELAY_RELAY_EN_IOMUX);
  DL_GPIO_enableOutput(RELAY_PORT, RELAY_RELAY_EN_PIN);
  DL_GPIO_clearPins(RELAY_PORT, RELAY_RELAY_EN_PIN);

  DL_GPIO_initDigitalOutput(SWITCH_SWITCH_EN_IOMUX);
  DL_GPIO_enableOutput(SWITCH_PORT, SWITCH_SWITCH_EN_PIN);
  DL_GPIO_clearPins(SWITCH_PORT, SWITCH_SWITCH_EN_PIN);
}

void disable_all(void) {
  switch_off();
  relay_off();
}
void switch_on(void) { DL_GPIO_setPins(SWITCH_PORT, SWITCH_SWITCH_EN_PIN); }

void switch_off(void) { DL_GPIO_clearPins(SWITCH_PORT, SWITCH_SWITCH_EN_PIN); }

void relay_on(void) { DL_GPIO_setPins(RELAY_PORT, RELAY_RELAY_EN_PIN); }

void relay_off(void) { DL_GPIO_clearPins(RELAY_PORT, RELAY_RELAY_EN_PIN); }