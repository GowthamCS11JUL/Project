#include "adc_mux.h"
#include "headers.h"
#include "ti/driverlib/dl_gpio.h"

/* =========================
   INIT
   ========================= */

static void MUX1_init(void)
{
    /* EN̅ (GPIOA2) */
    DL_GPIO_initDigitalOutput(ADC_MUX_EN_ADC_MUX_EN_BAR_0_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_EN_PORT,
                         ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);

    /* S0–S3 */
    DL_GPIO_initDigitalOutput(ADC_MUX_0_ADC_MUX_0_S0_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_0_ADC_MUX_0_S0_PORT,
                         ADC_MUX_0_ADC_MUX_0_S0_PIN);

    DL_GPIO_initDigitalOutput(ADC_MUX_0_ADC_MUX_0_S1_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_0_ADC_MUX_0_S1_PORT,
                         ADC_MUX_0_ADC_MUX_0_S1_PIN);

    DL_GPIO_initDigitalOutput(ADC_MUX_0_ADC_MUX_0_S2_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_0_ADC_MUX_0_S2_PORT,
                         ADC_MUX_0_ADC_MUX_0_S2_PIN);

    DL_GPIO_initDigitalOutput(ADC_MUX_0_ADC_MUX_0_S3_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_0_ADC_MUX_0_S3_PORT,
                         ADC_MUX_0_ADC_MUX_0_S3_PIN);

    /* Disable initially */
    DL_GPIO_setPins(ADC_MUX_EN_PORT,
                    ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);
}

static void MUX2_init(void)
{
    /* EN̅ (GPIOA3) */
    DL_GPIO_initDigitalOutput(ADC_MUX_EN_ADC_MUX_EN_BAR_1_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_EN_PORT,
                         ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);

    /* S0–S3 */
    DL_GPIO_initDigitalOutput(ADC_MUX_1_ADC_MUX_1_S0_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_1_ADC_MUX_1_S0_PORT,
                         ADC_MUX_1_ADC_MUX_1_S0_PIN);

    DL_GPIO_initDigitalOutput(ADC_MUX_1_ADC_MUX_1_S1_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_1_ADC_MUX_1_S1_PORT,
                         ADC_MUX_1_ADC_MUX_1_S1_PIN);

    DL_GPIO_initDigitalOutput(ADC_MUX_1_ADC_MUX_1_S2_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_1_ADC_MUX_1_S2_PORT,
                         ADC_MUX_1_ADC_MUX_1_S2_PIN);

    DL_GPIO_initDigitalOutput(ADC_MUX_1_ADC_MUX_1_S3_IOMUX);
    DL_GPIO_enableOutput(ADC_MUX_1_ADC_MUX_1_S3_PORT,
                         ADC_MUX_1_ADC_MUX_1_S3_PIN);

    /* Disable initially */
    DL_GPIO_setPins(ADC_MUX_EN_PORT,
                    ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);
}

void mux_init_all(void)
{
    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);

    MUX1_init();
    MUX2_init();

    mux_disable_all();
}

/* =========================
   DISABLE
   ========================= */

void mux_disable_all(void)
{
    DL_GPIO_setPins(ADC_MUX_EN_PORT,
                    ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);

    DL_GPIO_setPins(ADC_MUX_EN_PORT,
                    ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);
}

void mux_disable(mux_id_t id)
{
    switch (id)
    {
    case MUX_ID_1:
        DL_GPIO_setPins(ADC_MUX_EN_PORT,
                        ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);
        break;

    case MUX_ID_2:
        DL_GPIO_setPins(ADC_MUX_EN_PORT,
                        ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);
        break;
    }
}

/* =========================
   SET SELECT LINES
   ========================= */

static inline void mux1_set(uint8_t ch)
{
    ch &= 0x0F;

    DL_GPIO_writePinsVal(ADC_MUX_0_ADC_MUX_0_S0_PORT,
                         ADC_MUX_0_ADC_MUX_0_S0_PIN,
                         (ch & 0x01) ? ADC_MUX_0_ADC_MUX_0_S0_PIN : 0);

    DL_GPIO_writePinsVal(ADC_MUX_0_ADC_MUX_0_S1_PORT,
                         ADC_MUX_0_ADC_MUX_0_S1_PIN,
                         (ch & 0x02) ? ADC_MUX_0_ADC_MUX_0_S1_PIN : 0);

    DL_GPIO_writePinsVal(ADC_MUX_0_ADC_MUX_0_S2_PORT,
                         ADC_MUX_0_ADC_MUX_0_S2_PIN,
                         (ch & 0x04) ? ADC_MUX_0_ADC_MUX_0_S2_PIN : 0);

    DL_GPIO_writePinsVal(ADC_MUX_0_ADC_MUX_0_S3_PORT,
                         ADC_MUX_0_ADC_MUX_0_S3_PIN,
                         (ch & 0x08) ? ADC_MUX_0_ADC_MUX_0_S3_PIN : 0);
}

static inline void mux2_set(uint8_t ch)
{
    ch &= 0x0F;

    DL_GPIO_writePinsVal(ADC_MUX_1_ADC_MUX_1_S0_PORT,
                         ADC_MUX_1_ADC_MUX_1_S0_PIN,
                         (ch & 0x01) ? ADC_MUX_1_ADC_MUX_1_S0_PIN : 0);

    DL_GPIO_writePinsVal(ADC_MUX_1_ADC_MUX_1_S1_PORT,
                         ADC_MUX_1_ADC_MUX_1_S1_PIN,
                         (ch & 0x02) ? ADC_MUX_1_ADC_MUX_1_S1_PIN : 0);

    DL_GPIO_writePinsVal(ADC_MUX_1_ADC_MUX_1_S2_PORT,
                         ADC_MUX_1_ADC_MUX_1_S2_PIN,
                         (ch & 0x04) ? ADC_MUX_1_ADC_MUX_1_S2_PIN : 0);

    DL_GPIO_writePinsVal(ADC_MUX_1_ADC_MUX_1_S3_PORT,
                         ADC_MUX_1_ADC_MUX_1_S3_PIN,
                         (ch & 0x08) ? ADC_MUX_1_ADC_MUX_1_S3_PIN : 0);
}

/* =========================
   SELECT (INDIVIDUAL CONTROL)
   ========================= */

void mux_select(mux_id_t id, uint8_t channel)
{
    channel &= 0x0F;

    switch (id)
    {
    case MUX_ID_1:
        mux1_set(channel);
        for (volatile int i = 0; i < 20; i++);
        DL_GPIO_clearPins(ADC_MUX_EN_PORT,
                          ADC_MUX_EN_ADC_MUX_EN_BAR_0_PIN);
        break;

    case MUX_ID_2:
        mux2_set(channel);
        for (volatile int i = 0; i < 20; i++);
        DL_GPIO_clearPins(ADC_MUX_EN_PORT,
                          ADC_MUX_EN_ADC_MUX_EN_BAR_1_PIN);
        break;
    }
}