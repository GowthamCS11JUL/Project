#include "adc_mux.h"
#include "headers.h"
#include "ti/driverlib/dl_gpio.h"

/* =========================
   INIT FUNCTIONS
   ========================= */
void mux_init_all(void) {
  DL_GPIO_enablePower(GPIOA);
  DL_GPIO_enablePower(GPIOB);

  mux_0_init();
  mux_1_init();
}

void mux_0_init(void) {
  /* Configure EN (Active LOW) */
  DL_GPIO_initDigitalOutput(ADC_MUX_EN_ADC_MUX_EN_BAR_0_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);

  /* Configure S lines */
  DL_GPIO_initDigitalOutput(ADC_MUX_0_ADC_MUX_0_S0_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_0_ADC_MUX_0_S0_PORT, ADC_MUX_0_ADC_MUX_0_S0_PIN);

  DL_GPIO_initDigitalOutput(ADC_MUX_0_ADC_MUX_0_S1_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_0_ADC_MUX_0_S1_PORT, ADC_MUX_0_ADC_MUX_0_S1_PIN);

  DL_GPIO_initDigitalOutput(ADC_MUX_0_ADC_MUX_0_S2_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_0_ADC_MUX_0_S2_PORT, ADC_MUX_0_ADC_MUX_0_S2_PIN);

  DL_GPIO_initDigitalOutput(ADC_MUX_0_ADC_MUX_0_S3_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_0_ADC_MUX_0_S3_PORT, ADC_MUX_0_ADC_MUX_0_S3_PIN);

  /* Disable mux initially */
  DL_GPIO_setPins(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);
}

void mux_1_init(void) {
  /* Configure EN (Active LOW) */
  DL_GPIO_initDigitalOutput(ADC_MUX_EN_ADC_MUX_EN_BAR_1_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);

  /* Configure S lines */
  DL_GPIO_initDigitalOutput(ADC_MUX_1_ADC_MUX_1_S0_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_1_ADC_MUX_1_S0_PORT, ADC_MUX_1_ADC_MUX_1_S0_PIN);

  DL_GPIO_initDigitalOutput(ADC_MUX_1_ADC_MUX_1_S1_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_1_ADC_MUX_1_S1_PORT, ADC_MUX_1_ADC_MUX_1_S1_PIN);

  DL_GPIO_initDigitalOutput(ADC_MUX_1_ADC_MUX_1_S2_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_1_ADC_MUX_1_S2_PORT, ADC_MUX_1_ADC_MUX_1_S2_PIN);

  DL_GPIO_initDigitalOutput(ADC_MUX_1_ADC_MUX_1_S3_IOMUX);
  DL_GPIO_enableOutput(ADC_MUX_1_ADC_MUX_1_S3_PORT, ADC_MUX_1_ADC_MUX_1_S3_PIN);

  /* Disable mux initially */
  DL_GPIO_setPins(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);
}

/* =========================
   DISABLE FUNCTIONS
   ========================= */
void mux_disable(mux_id id) {
  switch (id) {
  case MUX_0:
    DL_GPIO_setPins(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);
    break;

  case MUX_1:
    DL_GPIO_setPins(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);
    break;

  default:
    break;
  }
}

void mux_disable_all(void) {
  DL_GPIO_setPins(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);
  DL_GPIO_setPins(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);
}

/* =========================
   CHANNEL SELECT (LOW LEVEL)
   ========================= */
static inline void mux_0_set(uint8_t ch) {
  ch &= 0x0F;

  DL_GPIO_writePinsVal(ADC_MUX_0_ADC_MUX_0_S0_PORT, ADC_MUX_0_ADC_MUX_0_S0_PIN,
                       (ch & 0x01) ? ADC_MUX_0_ADC_MUX_0_S0_PIN : 0);

  DL_GPIO_writePinsVal(ADC_MUX_0_ADC_MUX_0_S1_PORT, ADC_MUX_0_ADC_MUX_0_S1_PIN,
                       (ch & 0x02) ? ADC_MUX_0_ADC_MUX_0_S1_PIN : 0);

  DL_GPIO_writePinsVal(ADC_MUX_0_ADC_MUX_0_S2_PORT, ADC_MUX_0_ADC_MUX_0_S2_PIN,
                       (ch & 0x04) ? ADC_MUX_0_ADC_MUX_0_S2_PIN : 0);

  DL_GPIO_writePinsVal(ADC_MUX_0_ADC_MUX_0_S3_PORT, ADC_MUX_0_ADC_MUX_0_S3_PIN,
                       (ch & 0x08) ? ADC_MUX_0_ADC_MUX_0_S3_PIN : 0);
}

static inline void mux_1_set(uint8_t ch) {
  ch &= 0x0F;

  DL_GPIO_writePinsVal(ADC_MUX_1_ADC_MUX_1_S0_PORT, ADC_MUX_1_ADC_MUX_1_S0_PIN,
                       (ch & 0x01) ? ADC_MUX_1_ADC_MUX_1_S0_PIN : 0);

  DL_GPIO_writePinsVal(ADC_MUX_1_ADC_MUX_1_S1_PORT, ADC_MUX_1_ADC_MUX_1_S1_PIN,
                       (ch & 0x02) ? ADC_MUX_1_ADC_MUX_1_S1_PIN : 0);

  DL_GPIO_writePinsVal(ADC_MUX_1_ADC_MUX_1_S2_PORT, ADC_MUX_1_ADC_MUX_1_S2_PIN,
                       (ch & 0x04) ? ADC_MUX_1_ADC_MUX_1_S2_PIN : 0);

  DL_GPIO_writePinsVal(ADC_MUX_1_ADC_MUX_1_S3_PORT, ADC_MUX_1_ADC_MUX_1_S3_PIN,
                       (ch & 0x08) ? ADC_MUX_1_ADC_MUX_1_S3_PIN : 0);
}

/* =========================
   MAIN SELECT FUNCTION
   ========================= */
void mux_select(mux_id id, uint8_t channel) {
  channel &= 0x0F;

  /* Step 1: Disable all muxes (avoid contention) */
  mux_disable_all();

  /* Step 2: Set channel */
  switch (id) {
  case MUX_0:
    mux_0_set(channel);
    break;

  case MUX_1:
    mux_1_set(channel);
    break;

  default:
    return;
  }

  /* Step 3: Small stabilization delay */
  for (volatile int i = 0; i < 20; i++)
    ;

  /* Step 4: Enable selected mux (Active LOW) */
  switch (id) {
  case MUX_0:
    DL_GPIO_clearPins(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);
    break;

  case MUX_1:
    DL_GPIO_clearPins(ADC_MUX_EN_PORT, ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);
    break;

  default:
    break;
  }
}