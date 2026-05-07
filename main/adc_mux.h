#ifndef ADC_MUX_H_
#define ADC_MUX_H_

#include <stdint.h>

typedef enum
{
    MUX_ID_1 = 0,
    MUX_ID_2
} mux_id_t;

/* Init */
void mux_init_all(void);

/* Control */
void mux_disable_all(void);
void mux_disable(mux_id_t id);

/* Select */
void mux_select(mux_id_t id, uint8_t channel);

#endif