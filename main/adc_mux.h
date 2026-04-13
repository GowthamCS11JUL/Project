#ifndef ADC_MUX_H_
#define ADC_MUX_H_
#include <stdint.h>
typedef enum 
{
    MUX_0=0,
    MUX_1
}mux_id;

void mux_init_all(void);//To initialise all mux 

void mux_0_init(void);

void mux_1_init(void);


void mux_disable_all(void);//To disbale all mux

void mux_disable(mux_id id);//To disable particular mux

/* Select channel (0–15) from given mux */
static inline void mux_0_set(uint8_t ch);
static inline void mux_1_set(uint8_t ch);

void mux_select(mux_id id,uint8_t channel);//To select the particular mux

#endif
