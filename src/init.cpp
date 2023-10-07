#include "stm32f4.h"

void stm32f4_hh::init_chip()
{
    const struct rcc_clock_scale rcc_hse_24mhz_3v3
    {
        // These are defined in CUBEIDE clock config
        .pllm = 24, // This is the important one that divides input clock by 24, will allow us to match clocks of any of the libopencm3 examples
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
        // These are the set values for HCLK and APB peripheralsS
            .ahb_frequency = 84000000,
        .apb1_frequency = 42000000,
        .apb2_frequency = 84000000,
    };

    rcc_clock_setup_pll(&rcc_hse_24mhz_3v3);
    rcc_periph_clock_enable(RCC_GPIOA);
}

void stm32f4_hh::init_can()
{

}

void stm32f4_hh::hytech_print(std::string &out){}