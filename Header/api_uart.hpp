#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <span>

#include "uart_config.hpp"


namespace Uart {

enum class Status : uint8_t { OK = 0, ERROR };

enum class Interface : uint8_t {
  U1 = 1,
  U2 = 2,
  U3 = 3,
  U4 = 4,
  U5 = 5,
  U6 = 6
};

enum class Baudrate : uint32_t {
  B_0 = 0,
  B_1200 = 1200,
  B_2400 = 2400,
  B_4800 = 4800,
  B_9600 = 9600,
  B_19200 = 19200,
  B_38400 = 38400,
  B_57600 = 57600,
  B_74880 = 74880,
  B_115200 = 115200,
  B_230400 = 230400,
  B_250000 = 250000
};

enum class Parity : uint8_t { NONE = 0, ODD = 1, EVEN = 2 };

enum class Stopbits : uint8_t { ONE = 1, TWO = 2 };

enum class Mode : uint8_t { TX_ONLY = 0, RX_ONLY = 1, TX_RX = 2 };

struct Params {
  Baudrate baudrate;
  Parity parity;
  Stopbits stopbits;
  Mode mode;
  uint8_t rxPacketSize;
  std::function<void(std::span<uint8_t>)> receivedCallback;
  std::function<void()> transmitCompletedCallback;
};

Status init(Interface interface, const Params &init);
Status deinit(Interface interface);
Status changeBaudrate(Interface interface, Baudrate baudrate);
Status changeParity(Interface interface, Parity parity);
Status changeStopbits(Interface interface, Stopbits stopbits);
Status changeReceivedCallback(Interface interface, std::function<void(std::span<uint8_t>)> callback);
Status changeTransmitCompletedCallback(Interface interface, std::function<void()> callback);
Status sendBytes(std::span<uint8_t> data);
}  // namespace Uart

Uart::Baudrate operator""_bod(unsigned long long value);
