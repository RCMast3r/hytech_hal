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
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dac.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

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

#define USART_CONSOLE USART2

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

int _write(int file, char *ptr, int len);

static void clock_setup(void)
{
	rcc_clock_setup_pll(&rcc_hse_24mhz_3v3);
	/* Enable GPIOD clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2 and dac */
	rcc_periph_clock_enable(RCC_USART2);
	rcc_periph_clock_enable(RCC_DAC);

	/* And ADC*/
	rcc_periph_clock_enable(RCC_ADC1);
}

static void usart_setup(void)
{
	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0);

	/* Setup USART2 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO0);

	usart_set_baudrate(USART_CONSOLE, 115200);
	usart_set_databits(USART_CONSOLE, 8);
	usart_set_stopbits(USART_CONSOLE, USART_STOPBITS_1);
	usart_set_mode(USART_CONSOLE, USART_MODE_TX);
	usart_set_parity(USART_CONSOLE, USART_PARITY_NONE);
	usart_set_flow_control(USART_CONSOLE, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART_CONSOLE);
}

/**
 * Use USART_CONSOLE as a console.
 * This is a syscall for newlib
 * @param file
 * @param ptr
 * @param len
 * @return
 */
int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				usart_send_blocking(USART_CONSOLE, '\r');
			}
			usart_send_blocking(USART_CONSOLE, ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}

static void adc_setup(void)
{
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0);
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);

	adc_power_off(ADC1);
	adc_disable_scan_mode(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);

	adc_power_on(ADC1);

}

static void dac_setup(void)
{
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO5);
	dac_disable(DAC1, DAC_CHANNEL2);
	dac_disable_waveform_generation(DAC1, DAC_CHANNEL2);
	dac_enable(DAC1, DAC_CHANNEL2);
	dac_set_trigger_source(DAC1, DAC_CR_TSEL2_SW);
}

static uint16_t read_adc_naiive(uint8_t channel)
{
	uint8_t channel_array[16];
	channel_array[0] = channel;
	adc_set_regular_sequence(ADC1, 1, channel_array);
	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	uint16_t reg16 = adc_read_regular(ADC1);
	return reg16;
}

int main(void)
{
	int i;
	int j = 0;
	clock_setup();
	usart_setup();
	printf("hi guys!\n");

	/* green led for ticking */
	gpio_mode_setup(LED_DISCO_GREEN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
			LED_DISCO_GREEN_PIN);

	while (1) {

		printf("hi guys!\n");

		/* LED on/off */
		gpio_toggle(LED_DISCO_GREEN_PORT, LED_DISCO_GREEN_PIN);

		for (i = 0; i < 1000000; i++) { /* Wait a bit. */
			__asm__("NOP");
		}
	}

	return 0;
}