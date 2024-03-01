#ifdef __cplusplus
extern "C" {
#endif

#include <giwdg.h>
#include <stm32yyxx_ll_iwdg.h>

void iwdg_init(int rl) {

  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);

  // 32kHz / 32 = 1kHz
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_32);

  // counter (999+1) is 1sec
  LL_IWDG_SetReloadCounter(IWDG, rl);

  while (LL_IWDG_IsReady(IWDG) != 1);
  LL_IWDG_ReloadCounter(IWDG);
}

void iwdg_clear() {
	LL_IWDG_ReloadCounter(IWDG);
}

void iwdg_set(uint16_t ms) {
  LL_IWDG_SetReloadCounter(IWDG, ms);
}

#ifdef __cplusplus
}
#endif
