/*
 * Module: MCU
 * File Name: MCU.h
 * Description: Source file for TM4C123GH6PM Microcontroller - Mcu Driver
 * Author: Esraa Khaled
 */


#include "tm4c123gh6pm_registers.h"

void Mcu_Init(void)
{
    /*Enable Clock for all ports and wait for the clock to start*/
    SYSCTL_RCGCGPIO_REG |= 0x3F;
    while(!(SYSCTL_PRGPIO_REG & 0x3F));
}


