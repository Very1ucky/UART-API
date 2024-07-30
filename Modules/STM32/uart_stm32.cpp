#include "api_uart.hpp"
#include "usart.h"

using namespace Uart;

static UART_HandleTypeDef* getUart(Interface interface);

Status sendBytes(std::span<const uint8_t> data) { return Status::OK; }

Status init(Interface interface, const Params& init) {
  if (init.baudrate == Baudrate::B_0) {
    return Status::ERROR;
  }

  UART_HandleTypeDef* uart = getUart(interface);
  if (uart == nullptr) {
    return Status::ERROR;
  }

  uart->Init.BaudRate = static_cast<uint32_t>(init.baudrate);
  uart->Init.WordLength = UART_WORDLENGTH_8B;
  uart->Init.StopBits =
      init.stopbits == Stopbits::ONE ? UART_STOPBITS_1 : UART_STOPBITS_2;
  uart->Init.Parity =
      init.parity == Parity::NONE
          ? UART_PARITY_NONE
          : (init.parity == Parity::ODD ? UART_PARITY_ODD : UART_PARITY_EVEN);
  uart->Init.Mode =
      init.mode == Mode::TX_ONLY
          ? UART_MODE_TX
          : (init.mode == Mode::RX_ONLY ? UART_MODE_RX : UART_MODE_TX_RX);
  uart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart->Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(uart) != HAL_OK) {
    return Status::ERROR;
  }

  return Status::OK;
}

static UART_HandleTypeDef* getUart(Interface interface) {
  switch (interface) {
#ifdef U1_ENABLED
    case Interface::U1:
      huart1.Instance = USART1;
      return &huart1;
#endif

#ifdef U2_ENABLED
    case Interface::U2:
      huart2.Instance = USART2;
      return &huart2;
#endif

#ifdef U3_ENABLED
    case Interface::U3:
      huart3.Instance = USART3;
      return &huart3;
#endif

#ifdef U4_ENABLED
    case Interface::U4:
      huart4.Instance = UART4;
      return &huart4;
#endif

#ifdef U5_ENABLED
    case Interface::U5:
      huart5.Instance = UART5;
      return &huart5;
#endif

#ifdef U6_ENABLED
    case Interface::U6:
      huart6.Instance = UART6;
      return &huart6;
#endif
    default:
      return nullptr;
  }
}