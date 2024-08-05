/**
 * @file utils.cpp
 * @author Daniil (sukhikhDaniilA@yandex.ru)
 * @brief Utils platform-independent api functions
 * @version 1
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "api_uart.hpp"

using namespace Uart;

Baudrate operator""_bod(unsigned long long value) {
  switch (value) {
    case 1200: return Baudrate::B_1200;
    case 2400: return Baudrate::B_2400;
    case 4800: return Baudrate::B_4800;
    case 9600: return Baudrate::B_9600;
    case 19200: return Baudrate::B_19200;
    case 38400: return Baudrate::B_38400;
    case 57600: return Baudrate::B_57600;
    case 74880: return Baudrate::B_74880;
    case 115200: return Baudrate::B_115200;
    case 230400: return Baudrate::B_230400;
    case 250000: return Baudrate::B_250000;
    default: return Baudrate::B_0;
  }
}