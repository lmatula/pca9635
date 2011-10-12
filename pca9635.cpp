#include "pca9635.h"

// Constructor
pca9635::pca9635()
{
}

// Destructor
pca9635::~pca9635()
{
}

/**
 * Control the led channel mode, modes: 
 * 0=fully off
 * 1=fully on (no PWM)
 * 2=individual PWM only
 * 3=individual and group PWM
 *
 * Remember that led numbers start from 0
 */
boolean pca9635::set_led_mode(byte ledno, byte mode)
{
    byte reg = 0x17;
    // PONDER: Is there a more optimized way
    if (8 < ledno < 11)
    {
        reg = 0x16;
    }
    if (4 < ledno < 8)
    {
        reg = 0x15;
    }
    if (ledno < 4)
    {
        reg = 0x14;
    }
    byte value = 0;
    switch (mode)
    {
        case 0:
            value = B00;
            break;
        case 1:
            value = B01;
            break;
        case 2:
            value = B10;
            break;
        case 3:
            value = B11;
            break;
    }
    byte mask = B00000000;
    switch(ledno%4)
    {
        case 0:
            mask = B00000011;
            break;
        case 1:
            mask = B00001100;
            break;
        case 2:
            mask = B00110000;
            break;
        case 3:
            mask = B11000000;
            break;
    }
    return this->read_modify_write(reg, mask, value);
}

boolean pca9635::set_driver_mode(byte mode)
{
    return this->read_modify_write(0x01, _BV(2), mode << 2);
}

boolean pca9635::set_sleep(byte sleep)
{
    return this->read_modify_write(0x00, B00010000, sleep << 4 | ~_BV(4));
}


/**
 * Sets the pwm value for given led, note that it must have previously been enabled for PWM control with set_mode
 * 
 * Remember that led numbers start from 0
 */
boolean pca9635::set_led_pwm(byte ledno, byte cycle)
{
    byte reg = 0x02 + ledno;
    return this->write_many(reg, 1, &cycle);
}