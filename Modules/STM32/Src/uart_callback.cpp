#include "uart_callback.hpp"


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    auto handles = getUartHandles();
    for (auto& handle : handles) {
        if (handle.handle == huart && handle.txCallback != nullptr) {
            handle.txCallback();
            break;
        }
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    auto handles = getUartHandles();
    for (auto& handle : handles) {
        if (handle.handle == huart && handle.rxCallback != nullptr) {
            handle.rxCallback(std::span{handle.buffer}.first(handle.rxPacketSize));
            break;
        }
    }
}