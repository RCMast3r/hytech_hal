#pragma once

#include <string>

class hytech_hal
{

    // setup clocks and enable pins
    // I am thinking that per-each chip we will have just one way of clock setup and
    virtual void init_chip() = 0;
    // initialize CAN peripheral
    // TODO 
    virtual void init_can() = 0;
    virtual void hytech_print(std::string &out) = 0; 
    
};
