#include "headers.h"

void Delay_Timer_Initialize(Delay_Timer *thisTimer, GPTIMER_Regs *thisTimerRegs,
                            uint8_t thisInstance, uint32_t clockFrequency) {
  thisTimer->timer = thisTimerRegs;
  thisTimer->instanceNum = thisInstance;
  thisTimer->overflowCount = 0;

  /* -----------------------------------------
   * Configure 1 MHz timer base
   * ----------------------------------------- */
  thisTimer->clkConfig.clockSel = DL_TIMER_CLOCK_BUSCLK;
  thisTimer->clkConfig.divideRatio = DL_TIMER_CLOCK_DIVIDE_1;
  thisTimer->clkConfig.prescale = (uint8_t)((clockFrequency / 1000000) - 1);

  DL_TimerG_setClockConfig(thisTimer->timer, &(thisTimer->clkConfig));

  /* -----------------------------------------
   * Periodic Up Mode
   * ----------------------------------------- */
  thisTimer->config.startTimer = DL_TIMER_STOP;
  thisTimer->config.timerMode = DL_TIMER_TIMER_MODE_PERIODIC_UP;
  thisTimer->config.period = 0xFFFF;

  DL_TimerG_initTimerMode(thisTimer->timer, &(thisTimer->config));

  DL_Timer_setCounterValueAfterEnable(thisTimer->timer,
                                      DL_TIMER_COUNT_AFTER_EN_ZERO);

  DL_TimerG_enableInterrupt(thisTimer->timer, DL_TIMERG_INTERRUPT_ZERO_EVENT);

  /* NVIC configuration */
  if (thisInstance == 0) {
    NVIC_ClearPendingIRQ(TIMG0_INT_IRQn);
    NVIC_EnableIRQ(TIMG0_INT_IRQn);
  } else if (thisInstance == 6) {
    NVIC_ClearPendingIRQ(TIMG6_INT_IRQn);
    NVIC_EnableIRQ(TIMG6_INT_IRQn);
  } else if (thisInstance == 7) {
    NVIC_ClearPendingIRQ(TIMG7_INT_IRQn);
    NVIC_EnableIRQ(TIMG7_INT_IRQn);
  } else if (thisInstance == 8) {
    NVIC_ClearPendingIRQ(TIMG8_INT_IRQn);
    NVIC_EnableIRQ(TIMG8_INT_IRQn);
  }
}

/* ==========================================
 * Start Timer
 * ========================================== */
void Delay_Timer_Start(Delay_Timer *thisTimer) {
  thisTimer->overflowCount = 0;

  DL_Timer_stopCounter(thisTimer->timer);

  DL_Timer_setCounterValueAfterEnable(thisTimer->timer,
                                      DL_TIMER_COUNT_AFTER_EN_ZERO);

  DL_Timer_startCounter(thisTimer->timer);
}

/* ==========================================
 * Get Current Time (in microseconds)
 * ========================================== */
uint32_t Delay_Timer_Get(Delay_Timer *thisTimer) {
  uint32_t overflows;
  uint16_t current;

  /* Read overflow and counter safely */
  overflows = thisTimer->overflowCount;
  current = DL_Timer_getTimerCount(thisTimer->timer);

  return (overflows * 65536UL) + current;
}

/* ==========================================
 * IRQ Handler
 * ========================================== */
void Delay_Timer_IRQ(Delay_Timer *thisTimer) {
  switch (DL_TimerG_getPendingInterrupt(thisTimer->timer)) {

  case DL_TIMER_IIDX_ZERO:
    thisTimer->overflowCount++;
    break;

  default:
    break;
  }
}