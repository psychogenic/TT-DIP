/*
 * Clock.cpp, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/platform/Clock.h"
#include "includes/settings.h"
#include "includes/Globals.h"

extern "C" {

#ifdef DESKTOP_DEBUG

#define PWM_PA3_Init(hz)			hz
#define Timer_Interrupt_Init(hz)	hz
#define Timer_Interrupt_Stop()
#define PWM_PA3_Stop()
#else
static uint8_t tog_count = 0;
void TIM3_IRQHandler(void) __attribute__((interrupt));
void TIM3_IRQHandler(void)
{

	TIM3->INTFR = ~TIM_FLAG_Update; // ~TIM_UIF;
	funDigitalWrite(TTDIP_PIN_CLOCK_PIN, tog_count & 1 ? FUN_HIGH : FUN_LOW);
	tog_count++;

}
#endif
}


namespace Platform {


#define DEBUG_CLOCK_OUT(...) printf(__VA_ARGS__)

#ifndef DESKTOP_DEBUG

#define TIMER_CLK_HZ ((uint32_t)FUNCONF_SYSTEM_CORE_CLOCK) // 48_000_000
#define TIM_MAXDIV   65536UL   // 16-bit: PSC/ARR each give a divisor of 1..65536

// Unsigned absolute difference
static inline uint32_t u32_diff(uint32_t a, uint32_t b)
{
	return (a > b) ? (a - b) : (b - a);
}

static uint32_t calc_timer_nm(uint32_t target_hz, uint32_t *out_N, uint32_t *out_M)
{
	if (target_hz < 1) target_hz = 1;

	uint32_t total_ticks = (TIMER_CLK_HZ + target_hz / 2) / target_hz; // round to nearest
	if (total_ticks < 1) total_ticks = 1;

	uint32_t N = (total_ticks + TIM_MAXDIV - 1) / TIM_MAXDIV; // ceil(total_ticks / 65536)
	if (N < 1) N = 1;
	if (N > TIM_MAXDIV) N = TIM_MAXDIV;

	uint32_t M = (total_ticks + N / 2) / N; // round to nearest
	if (M < 1) M = 1;
	if (M > TIM_MAXDIV) {
		N++;
		M = (total_ticks + N / 2) / N;
		if (M > TIM_MAXDIV) M = TIM_MAXDIV;
	}
	if (N > TIM_MAXDIV) N = TIM_MAXDIV;

	*out_N = N;
	*out_M = M;
	return TIMER_CLK_HZ / (N * M);
}

static uint32_t calc_pwm_nm(uint32_t target_hz, uint32_t *out_N, uint32_t *out_M)
{
	if (target_hz < 1) target_hz = 1;

	uint32_t total_ticks = (TIMER_CLK_HZ + target_hz / 2) / target_hz;
	if (total_ticks < 2) total_ticks = 2; // hardware floor: N=1, M=2 -> 24 MHz

	uint32_t N = (total_ticks + TIM_MAXDIV - 1) / TIM_MAXDIV;
	if (N < 1) N = 1;
	if (N > TIM_MAXDIV) N = TIM_MAXDIV;

	uint32_t M_raw = total_ticks / N;
	uint32_t M_lo  = (M_raw / 2) * 2;
	if (M_lo < 2) M_lo = 2;
	uint32_t M_hi  = M_lo + 2;
	if (M_hi > TIM_MAXDIV) M_hi = TIM_MAXDIV;

	uint32_t f_lo = TIMER_CLK_HZ / (N * M_lo);
	uint32_t f_hi = TIMER_CLK_HZ / (N * M_hi);
	uint32_t M = (u32_diff(f_lo, target_hz) <= u32_diff(f_hi, target_hz)) ? M_lo : M_hi;

	if (M > TIM_MAXDIV) {
		N++;
		M_raw = total_ticks / N;
		M_lo  = (M_raw / 2) * 2;
		if (M_lo < 2) M_lo = 2;
		M_hi  = M_lo + 2;
		if (M_hi > TIM_MAXDIV) M_hi = TIM_MAXDIV;
		f_lo = TIMER_CLK_HZ / (N * M_lo);
		f_hi = TIMER_CLK_HZ / (N * M_hi);
		M = (u32_diff(f_lo, target_hz) <= u32_diff(f_hi, target_hz)) ? M_lo : M_hi;
	}
	if (N > TIM_MAXDIV) N = TIM_MAXDIV;
	if (M < 2) M = 2;
	if (M > TIM_MAXDIV) M = TIM_MAXDIV;

	*out_N = N;
	*out_M = M;
	return TIMER_CLK_HZ / (N * M);
}

/*
 * 50% duty cycle PWM output on PA3, TIM2 Channel 4 (PA3 = TIM2_CH4, no
 * remap).
 * Returns actual frequency set.
 */
uint32_t PWM_PA3_Init(uint32_t frequency_hz)
{
	uint32_t N, M;
	uint32_t actual_hz = calc_pwm_nm(frequency_hz, &N, &M);

	// Clocks: TIM2 is on APB1; the alternate-function pin needs AFIO + its
	// GPIO port clock. USBFS on this chip only ever enables AFIO + GPIOC,
	// so enabling GPIOA here can't collide with it.
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA;
	RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

	// PA3 as alternate-function push-pull output. 50 MHz slew setting so
	// the pin can keep up across the whole supported frequency range.
	funPinMode(PA3, GPIO_CFGLR_OUT_50Mhz_AF_PP);

	TIM2->CTLR1  = 0;
	TIM2->PSC    = (uint16_t)(N - 1);
	TIM2->ATRLR  = (uint16_t)(M - 1);   // auto-reload register: period = ARR+1 ticks
	TIM2->CH4CVR = M / 2;               // exact 50% duty (M is always even)

	// Channel 4: PWM Mode 1 (0b110), with preload so CH4CVR only updates
	// on the next timer update event.
	TIM2->CHCTLR2 = (TIM2->CHCTLR2 & ~(0x7u << 12)) | (0x6u << 12) | TIM_OC4PE;
	TIM2->CTLR1  |= TIM_ARPE;            // auto-reload preload enable

	TIM2->CCER = (TIM2->CCER & ~TIM_CC4P) | TIM_CC4E; // enable ch4 output, active-high

	TIM2->BDTR   |= TIM_MOE;   // main output enable (present on this chip's TIM2 too)
	TIM2->SWEVGR  = TIM_UG;    // force an update event so PSC/ATRLR load immediately
	TIM2->CTLR1  |= TIM_CEN;   // start the timer

	return actual_hz;
}

void PWM_PA3_Stop(void)
{
    // Disable the channel output, pin goes to the inactive state
    TIM2->CCER &= ~TIM_CC4E;

    // Clear main output enable
    TIM2->BDTR &= ~TIM_MOE;

    // Stop the counter
    TIM2->CTLR1 &= ~TIM_CEN;

    // Clear any pending update flag
    TIM2->INTFR = ~TIM_UIF;
}


uint32_t Timer_Interrupt_Init(uint32_t frequency_hz)
{
	uint32_t N, M;
	uint32_t actual_hz = calc_timer_nm(frequency_hz*2, &N, &M);

	RCC->APB1PCENR |= RCC_APB1Periph_TIM3;

	TIM3->CTLR1 = 0;
	TIM3->PSC   = (uint16_t)(N - 1);
	TIM3->ATRLR = (uint16_t)(M - 1);
	TIM3->CTLR1 |= TIM_ARPE;

	TIM3->SWEVGR    = TIM_UG;   // load PSC/ATRLR now
	TIM3->INTFR     = 0;        // clear any stale flags (the UG above sets UIF)
	TIM3->DMAINTENR = TIM_UIE;  // enable the update interrupt

	NVIC_EnableIRQ(TIM3_IRQn);

	TIM3->CTLR1 |= TIM_CEN;     // start the timer

	return actual_hz/2;
}


void Timer_Interrupt_Stop(void)
{
    // Prevent further update events from generating interrupts
    TIM3->DMAINTENR &= ~TIM_UIE;

    // Stop the counter
    TIM3->CTLR1 &= ~TIM_CEN;

    // Clear any pending update flag (UG or overflow may have set UIF)
    TIM3->INTFR = ~TIM_UIF;

    // mask the IRQ in the NVIC
    NVIC_DisableIRQ(TIM3_IRQn);
}

#endif


Clock::Clock() : clock_rate_hz(0)
{

}


ClockRateHz Clock::start(ClockRateHz hz)
{
	if (SystemSettings.clock_rst_driven_externally) {
		return 0;
	}
	if (hz == 0) {
		stop();
		return 0;
	}

#ifdef DESKTOP_DEBUG
	DEBUG_CLOCK_OUT("(Not) clocking at %lu Hz\r\n", (unsigned long)hz);
	clock_rate_hz = hz;
	return hz;
#endif

	// uint32_t pwm_actual = PWM_PA3_Init(1000); // 1 kHz, 50% duty, on PA3
	// printf("PWM_PA3_Init(1000) actual %lu Hz\n", (unsigned long)pwm_actual);
	TinyTapeout::Globals::chip()->modeOutput(TTDIP_PIN_CLOCK_PIN, GPIO_Speed_50MHz);
#ifdef TTDIP_CLOCK_USE_PA3
    uint32_t actual_hz = PWM_PA3_Init(hz);
#else
    uint32_t actual_hz = Timer_Interrupt_Init(hz);
#endif
	DEBUG_CLOCK_OUT("Clocking at %lu Hz\r\n", (unsigned long)actual_hz);

    return clock_rate_hz;
}
void Clock::stop(void)
{
	if (!clock_rate_hz) {
		return;
	}
#ifdef TTDIP_CLOCK_USE_PA3
	PWM_PA3_Stop();
#else
	Timer_Interrupt_Stop();
#endif

#ifdef TTDIP_CLOCK_PULLUP_ENABLE
	TinyTapeout::Globals::chip()->modeInputPull(TTDIP_PIN_CLOCK_PIN, true);
#else
	TinyTapeout::Globals::chip()->modeInput(TTDIP_PIN_CLOCK_PIN);
#endif
    clock_rate_hz = 0;
}

} /* namespace Platform */
