#pragma once

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/f4/rcc.h>
#ifdef __cplusplus
}
#endif

#include "hytech_hal.h"

class stm32f4_hh : public hytech_hal
{
    // initialize the clocks
    void init_chip() override;
    void init_usart() override;
    void init_can() override;
};