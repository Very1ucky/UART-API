/**
 * @file uart_stm32.hpp
 * @author Daniil (sukhikhDaniilA@yandex.ru)
 * @brief Header file for uart stm32 functions and structs
 * @version 1
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <ranges>
#include <algorithm>
#include "api_uart.hpp"
#include "usart.h"

struct UartHandle {
    UART_HandleTypeDef* handle;
    uint8_t rxPacketSize;
    std::array<uint8_t, MAX_RX_PACKET_SIZE> buffer = {0};
    std::function<void(std::span<uint8_t>)> rxCallback;
    std::function<void()> txCallback;
};

UART_HandleTypeDef* getUartSTMHandle(Uart::Interface interface);
UartHandle& getUartHandle(UART_HandleTypeDef* handle);