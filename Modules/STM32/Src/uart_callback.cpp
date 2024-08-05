/**
 * @file uart_callback.cpp
 * @author Daniil (sukhikhDaniilA@yandex.ru)
 * @brief STM32 HAL uart callback functions
 * @version 1
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "uart_callback.hpp"


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    auto& handle = getUartHandle(huart);
    if (handle.txCallback != nullptr) {
        handle.txCallback();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    auto& handle = getUartHandle(huart);
    if (handle.rxCallback != nullptr) {
        handle.rxCallback(std::span(handle.buffer.data(), handle.rxPacketSize));
    }

    HAL_UART_Receive_IT(handle.handle, handle.buffer.data(), handle.rxPacketSize);
}