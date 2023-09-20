/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2014 Karl Palsson <karlp@tweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#define LED_DISCO_GREEN_PORT GPIOA
#define LED_DISCO_GREEN_PIN GPIO3

// RCC clock defines
// #define RCC_CFGR_PLLSRC_HSE_CLK 0x1
// #define RCC_CFGR_HPRE_NODIV 0x0
// #define RCC_CFGR_PPRE_DIV2 0x4
// #define RCC_CFGR_PPRE_NODIV 0x0
// // #define PWR_SCALE1 0x3 This complains for some reason
#define FLASH_ACR_DCEN (1 << 10)
#define FLASH_ACR_ICEN (1 << 9)
#define FLASH_ACR_LATENCY_2WS 0x02

const struct rcc_clock_scale rcc_hse_24mhz_3v3{
	// These are defined in CUBEIDE clock config
	.pllm = 24, //This is the important one that divides input clock by 24, will allow us to match clocks of any of the libopencm3 examples
	.plln = 336,
	.pllp = 4,
	.pllq = 7,
	.pllr = 0,
	// This changes the PLL Source MUX to choose the HSE clock	
	.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
	// No clue what this does
	.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_2WS,
	// This changes the division values for AHB and APB clocks
	.hpre = RCC_CFGR_HPRE_NODIV,
	.ppre1 = RCC_CFGR_PPRE_DIV2,
	.ppre2 = RCC_CFGR_PPRE_NODIV,
	.voltage_scale = PWR_SCALE1,
	//These are the set values for HCLK and APB peripheralsS
	.ahb_frequency = 84000000,
	.apb1_frequency = 42000000,
	.apb2_frequency = 84000000,
};

// Defines for neopixel timer
typedef union
{
  struct
  {
    uint8_t w;
    uint8_t b;
    uint8_t r;
    uint8_t g;
  } color;
  uint32_t data;
} PixelRGBW_t;

#define NEOPIXEL_0		25
#define NEOPIXEL_1		51
#define NUM_PIXELS		3
#define DMA_BUF_SIZE	(NUM_PIXELS * 32) + 1

#define NEOPIXEL_PORT	GPIOA
#define NEOPIXEL_PIN	GPIO2

static void GPIO_setup(void) {

	gpio_mode_setup(LED_DISCO_GREEN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
				LED_DISCO_GREEN_PIN);

	

}

static void timer_setup(void) {

	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_clock_enable(RCC_GPIOA);

	// Sets gpio output to PWM Push-Pull configuration, not sure about 50MHZ
	gpio_set_output_options(NEOPIXEL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, NEOPIXEL_PIN);

	rcc_periph_clock_enable(RCC_TIM2);
	// Sets timer mode with clock division ratio, timer alignemnt (center or edge), and count direction
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1,
				TIM_CR1_DIR_UP);
	// Sets timer output compare mode with OC id and OC mode, it is set to pwm
	timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
	// Enables output compare
	timer_enable_oc_output(TIM2, TIM_OC1);
	// enables output in break (idk man, something about timer overflow)
	timer_enable_break_main_output(TIM2);
	// sets some random ass value, I have no clue what this means
	timer_set_oc_value(TIM2, TIM_OC1, 25);
	// Sets timer period
	timer_set_period(TIM2, 100);
	// enable counter
	timer_enable_counter(TIM2);
	// I have no clue how the above code sets channel unless it is automatic based on GPIO

}

static void clock_setup(void)
{
	rcc_clock_setup_pll(&rcc_hse_24mhz_3v3);
	/* Enable GPIOD clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2 and dac */
	rcc_periph_clock_enable(RCC_UART4);
	rcc_periph_clock_enable(RCC_DAC);

	/* And ADC*/
	rcc_periph_clock_enable(RCC_ADC1);
}

int main(void)
{
	int i;
	int j = 0;
	clock_setup();
	timer_setup();

	/* green led for ticking */	

	while (1) {

		// printf("hi guys!\n");

		// for(int i = 0; i < len(hello); i++) {
		// 	usart_send_blocking(hello[i]);
		// }
		// for (char ch: hello)
		// {
		// 	usart_send_blocking(USART_CONSOLE, ch);	
		// }
		

		/* LED on/off */
		gpio_toggle(LED_DISCO_GREEN_PORT, LED_DISCO_GREEN_PIN);

		for (i = 0; i < 1000000; i++) { /* Wait a bit. */
			__asm__("NOP");
		}
	}

	return 0;
}