#pragma once

#include <ranges>
#include <algorithm>
#include "api_uart.hpp"
#include "usart.h"

struct UartHandle {
    UART_HandleTypeDef* handle;
    uint8_t rxPacketSize;
    std::array<uint8_t, MAX_RX_PACKET_SIZE> buffer;
    std::function<void(std::span<uint8_t>)> rxCallback;
    std::function<void()> txCallback;
};

UART_HandleTypeDef* getUartSTMHandle(Uart::Interface interface);
std::vector<UartHandle>& getUartHandles();