#include "gtimer.h"
#include "stm32f1xx_hal_rcc.h"
#include "glog.h"

#ifdef __cplusplus

TIM_HandleTypeDef *htim1;
TIM_HandleTypeDef *htim2;
TIM_HandleTypeDef *htim3;
TIM_HandleTypeDef *htim4;

extern int timer_cnt;
void (*TIM_callback)(TIM_HandleTypeDef *) = 0;

void TIM1_IRQHandler(void) {
  HAL_TIM_IRQHandler(htim1);
	if( TIM_callback ) {
        TIM_callback(htim1);
    }
}

void TIM2_IRQHandler(void) {
  HAL_TIM_IRQHandler(htim2);
	if( TIM_callback ) {
        TIM_callback(htim2);
    }
}

void TIM3_IRQHandler(void) {
  HAL_TIM_IRQHandler(htim3);
	if( TIM_callback ) {
        TIM_callback(htim3);
    }
    timer_cnt++;
}

void TIM4_IRQHandler(void) {
  HAL_TIM_IRQHandler(htim4);
	if( TIM_callback ) {
        TIM_callback(htim4);
    }
}

#if 0
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if( TIM_callback ) {
        TIM_callback(htim);
    }
}
#endif

gtimer::gtimer() {
    _mode = 0;
    _ht.Instance = 0;;
}

gtimer::gtimer(TIM_TypeDef *TIMx, uint32_t PSC, uint32_t ARR, uint16_t mode) {
    set(TIMx, PSC,ARR,mode);
}

gtimer::~gtimer() {

}

void HUL_TIM_clk_enable( TIM_TypeDef* tim ) {
#ifdef TIM1
    if(tim == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE(); return;
    }
#endif
#ifdef TIM2
    if(tim == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE(); return;
    }
#endif
#ifdef TIM3
    if(tim == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE(); return;
    }
#endif
#ifdef TIM4
    if(tim == TIM4) {
        __HAL_RCC_TIM4_CLK_ENABLE(); return;
    }
#endif
}

void gtimer::set(TIM_TypeDef *TIMx, uint32_t PSC, uint32_t ARR, uint16_t mode) {
    TIM_Base_InitTypeDef bi;

    bi.ClockDivision = 0;
    bi.Period = ARR;
    bi.Prescaler = PSC;
    bi.CounterMode = TIM_COUNTERMODE_UP; 
    bi.RepetitionCounter = 0;
    bi.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    _ht.Instance = TIMx;
    _ht.Init = bi;

    _mode = mode;

    //psc(PSC);
    //arr(ARR);

    if (HAL_TIM_Base_Init(&_ht) != HAL_OK)
    {
        //Error_Handler();
    }
    HUL_TIM_clk_enable(_ht.Instance);

    start();
}

void gtimer::start() {
    HAL_TIM_Base_Start(&_ht);
}

void gtimer::stop() {
    HAL_TIM_Base_Stop(&_ht);
}

uint32_t gtimer::psc(int v) {
    if(v < 0) {
        return _ht.Instance->PSC;
    } else {
        _ht.Instance->PSC = v;
        return _ht.Instance->PSC;
    }
}

uint32_t gtimer::arr(int v) {
    if(v < 0) {
        return _ht.Instance->ARR;
    } else {
        _ht.Instance->ARR = v;
        return _ht.Instance->ARR;
    }
}

uint32_t gtimer::cnt(int v) {
    if(v < 0) {
        return _ht.Instance->CNT;
    } else {
        _ht.Instance->CNT = v;
        return _ht.Instance->CNT;
    }
}

uint32_t gtimer::ccr1(int v) {
    if(v < 0) {
        return _ht.Instance->CCR1;
    } else {
        _ht.Instance->CCR1 = v;
        return _ht.Instance->CCR1;
    }
}

uint32_t gtimer::ccr2(int v) {
    if(v < 0) {
        return _ht.Instance->CCR2;
    } else {
        _ht.Instance->CCR2 = v;
        return _ht.Instance->CCR2;
    }
}

uint32_t gtimer::ccr3(int v) {
    if(v < 0) {
        return _ht.Instance->CCR3;
    } else {
        _ht.Instance->CCR3 = v;
        return _ht.Instance->CCR3;
    }
}

uint32_t gtimer::ccr4(int v) {
    if(v < 0) {
        return _ht.Instance->CCR4;
    } else {
        _ht.Instance->CCR4 = v;
        return _ht.Instance->CCR4;
    }
}

gtimer_int::gtimer_int() {

}

gtimer_int::gtimer_int(TIM_TypeDef *TIMx, uint32_t PSC, uint32_t ARR, uint16_t mode, void (*f)(TIM_HandleTypeDef *))
    : gtimer(TIMx, PSC, ARR, mode) {

    set(TIMx, PSC, ARR, mode, f);
    start();
}


gtimer_int::~gtimer_int() {
    HAL_TIM_Base_Stop_IT(&_ht);
}

void gtimer_int::set(TIM_TypeDef *TIMx, uint32_t PSC, uint32_t ARR, uint16_t mode, void (*f)(TIM_HandleTypeDef *)) {
    gtimer::set(TIMx, PSC, ARR, mode);
    
    IRQn_Type irqn = (TIMx == TIM2)? TIM2_IRQn : (TIMx == TIM3)? TIM3_IRQn : (TIMx == TIM4)? TIM4_IRQn : (IRQn_Type)0;
    
    if( irqn ) {
        HAL_NVIC_SetPriority(irqn, 0, 0);
        HAL_NVIC_EnableIRQ(irqn);
    }

    if(f) attach(f);

    start();
   
    gdebug(2,"timer start (%d)\n",(int)irqn);
}

void gtimer_int::attach( void (*f)(TIM_HandleTypeDef *h)) {
    TIM_callback = f;
}

void gtimer_int::detach() {
    TIM_callback = 0;
}

void gtimer_int::start() {
    HAL_TIM_Base_Start_IT(&_ht);
}

void gtimer_int::stop() {
    HAL_TIM_Base_Stop_IT(&_ht);
}

#endif // __cplusplus