#include "uart_stm32.hpp"

using namespace Uart;

static std::vector<UartHandle> handles;

static Status validateUartParams(const Params& params);
static UartHandle& getUartHandle(Interface interface);
static Status addUartHandle(UartHandle& handle);
static Status removeUartHandle(UART_HandleTypeDef* handle);
static bool isRegistered(UART_HandleTypeDef* handle);

static Status startReceiving(UART_HandleTypeDef* handle);
static Status stopReceiving(UART_HandleTypeDef* handle, bool& wasStarted);

Status Uart::init(Interface interface, const Params& params) {
  if (validateUartParams(params) != Status::OK) {
    return Status::ERROR;
  }

  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  if (uart == nullptr || isRegistered(uart)) {
    return Status::ERROR;
  }

  uart->Init.BaudRate = static_cast<uint32_t>(params.baudrate);
  uart->Init.WordLength = UART_WORDLENGTH_8B;
  uart->Init.StopBits =
      params.stopbits == Stopbits::ONE ? UART_STOPBITS_1 : UART_STOPBITS_2;
  uart->Init.Parity =
      params.parity == Parity::NONE
          ? UART_PARITY_NONE
          : (params.parity == Parity::ODD ? UART_PARITY_ODD : UART_PARITY_EVEN);
  uart->Init.Mode =
      params.mode == Mode::TX_ONLY
          ? UART_MODE_TX
          : (params.mode == Mode::RX_ONLY ? UART_MODE_RX : UART_MODE_TX_RX);
  uart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart->Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(uart) != HAL_OK) {
    return Status::ERROR;
  }

  UartHandle newHandle{.handle = uart,
                       .rxPacketSize = params.rxPacketSize,
                       .rxCallback = params.receivedCallback,
                       .txCallback = params.transmitCompletedCallback};
  return addUartHandle(newHandle);
}

Status Uart::deinit(Interface interface) {
  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  bool wasStarted = false;
  if (stopReceiving(uart, wasStarted) != Status::OK) {
    return Status::ERROR;
  }

  if (HAL_UART_DeInit(uart) != HAL_OK) {
    return Status::ERROR;
  };

  return removeUartHandle(uart);
}

Status Uart::changeBaudrate(Interface interface, Baudrate baudrate) {
  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  bool wasStarted = false;
  if (stopReceiving(uart, wasStarted) != Status::OK) {
    return Status::ERROR;
  }

  uart->Init.BaudRate = static_cast<uint32_t>(baudrate);
  if (HAL_UART_Init(uart) != HAL_OK) {
    return Status::ERROR;
  }

  if (wasStarted) {
    return startReceiving(uart);
  }

  return Status::OK;
}

Status Uart::changeParity(Interface interface, Parity parity) {
  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  bool wasStarted = false;
  if (stopReceiving(uart, wasStarted) != Status::OK) {
    return Status::ERROR;
  }

  uart->Init.Parity =
      parity == Parity::NONE
          ? UART_PARITY_NONE
          : (parity == Parity::ODD ? UART_PARITY_ODD : UART_PARITY_EVEN);
  if (HAL_UART_Init(uart) != HAL_OK) {
    return Status::ERROR;
  }

  if (wasStarted) {
    return startReceiving(uart);
  }

  return Status::OK;
}

Status Uart::changeStopbits(Interface interface, Stopbits stopbits) {
  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  bool wasStarted = false;
  if (stopReceiving(uart, wasStarted) != Status::OK) {
    return Status::ERROR;
  }

  uart->Init.StopBits =
      stopbits == Stopbits::ONE ? UART_STOPBITS_1 : UART_STOPBITS_2;
  if (HAL_UART_Init(uart) != HAL_OK) {
    return Status::ERROR;
  }

  if (wasStarted) {
    return startReceiving(uart);
  }

  return Status::OK;
}

Status Uart::changeReceivedCallback(
    Interface interface, std::function<void(std::span<uint8_t>)> callback) {
  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  bool wasStarted = false;
  if (stopReceiving(uart, wasStarted) != Status::OK) {
    return Status::ERROR;
  }

  auto& uartHandle = getUartHandle(interface);
  uartHandle.rxCallback = callback;

  if (wasStarted) {
    return startReceiving(uart);
  }

  return Status::OK;
}

Status Uart::changeTransmitCompletedCallback(Interface interface,
                                       std::function<void()> callback) {
  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  bool wasStarted = false;
  if (stopReceiving(uart, wasStarted) != Status::OK) {
    return Status::ERROR;
  }

  auto& uartHandle = getUartHandle(interface);
  uartHandle.txCallback = callback;

  if (wasStarted) {
    return startReceiving(uart);
  }

  return Status::OK;
}

Status Uart::startReceiving(Interface interface) {
  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  return startReceiving(uart);
}

Status Uart::stopReceiving(Interface interface) {
  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  bool wasStarted = false;
  return stopReceiving(uart, wasStarted);
}

Status Uart::sendBytes(Interface interface, std::span<uint8_t> data) {
  UART_HandleTypeDef* uart = getUartSTMHandle(interface);
  if (HAL_UART_Transmit_IT(uart, data.data(), data.size()) != HAL_OK) {
    return Status::ERROR;
  }

  return Status::OK;
}

static Status startReceiving(UART_HandleTypeDef* handle) {
  if (handle == nullptr || !isRegistered(handle)) {
    return Status::ERROR;
  }

  auto& uartHandle = getUartHandle(handle);

  if (!isRegistered(uartHandle.handle)) {
    return Status::ERROR;
  }

  if (HAL_UART_Receive_IT(uartHandle.handle, uartHandle.buffer.data(),
                          uartHandle.rxPacketSize) != HAL_OK) {
    return Status::ERROR;
  }
  return Status::OK;
}

static Status stopReceiving(UART_HandleTypeDef* handle, bool& wasStarted) {
  if (handle == nullptr || !isRegistered(handle)) {
    return Status::ERROR;
  }

  wasStarted = handle->RxState == HAL_UART_STATE_BUSY_RX;

  if (HAL_UART_Abort_IT(handle) != HAL_OK) {
    return Status::ERROR;
  }
  return Status::OK;
}

UART_HandleTypeDef* getUartSTMHandle(Interface interface) {
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

static Status validateUartParams(const Params& params) {
  if (params.baudrate == Baudrate::B_0) {
    return Status::ERROR;
  }

  if (params.rxPacketSize > MAX_RX_PACKET_SIZE) {
    return Status::ERROR;
  }

  return Status::OK;
}

static Status addUartHandle(UartHandle& newHandle) {
  auto ret = std::ranges::find(handles, newHandle.handle, &UartHandle::handle);
  if (ret == handles.end()) {
    handles.push_back(newHandle);
    return Status::OK;
  }

  return Status::ERROR;
}

static Status removeUartHandle(UART_HandleTypeDef* handle) {
  const auto ret = std::ranges::remove(handles, handle, &UartHandle::handle);
  if (ret.size() == 0) {
    return Status::ERROR;
  }

  handles.erase(ret.begin(), ret.end());

  return Status::OK;
}

static bool isRegistered(UART_HandleTypeDef* handle) {
  return std::ranges::find(handles, handle, &UartHandle::handle) !=
         handles.end();
}

static UartHandle& getUartHandle(Interface interface) {
  return getUartHandle(getUartSTMHandle(interface));
}

UartHandle& getUartHandle(UART_HandleTypeDef* handle) {
  auto res = std::ranges::find(handles, handle, &UartHandle::handle);

  return *res;
}
