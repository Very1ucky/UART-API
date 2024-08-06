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

/**
 * @brief Gets the uart stm handle from Uart::Interface
 * 
 * @param interface : used interface
 * @return UART_HandleTypeDef* : pointer to stm hal uart handle, nullptr if not found
 */
UART_HandleTypeDef* getUartSTMHandle(Uart::Interface interface);
/**
 * @brief Gets the Uart Handle object from stm hal uart handle
 * 
 * @param handle : stm hal uart handle
 * @return UartHandle& : reference to UartHandle, ub if not found
 */
UartHandle& getUartHandle(UART_HandleTypeDef* handle);