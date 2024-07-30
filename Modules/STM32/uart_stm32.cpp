#include "api_uart.hpp"

Uart::Status Uart::sendArray(std::span<uint8_t> data) {
    return Status::OK;
}

Uart::Status Uart::init(const InitData &init) {
    return Status::OK;
}