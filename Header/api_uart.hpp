#pragma once

#include <cstdint>
#include <functional>
#include <array>
#include <span>

namespace Uart {
enum class Status : uint8_t { OK = 0, ERROR };

enum class Baudrate : uint32_t {
  B_0 = 0,
  B_1200 = 1200,
  B_2400 = 2400,
  B_4800 = 4800,
  B_9600 = 9600,
  B_19200 = 19200,
  B_38400 = 38400,
  B_57600 = 57600,
  B_115200 = 115200,
  B_256000 = 256000
};



enum class Parity : uint8_t { NONE = 0, ODD = 1, EVEN = 2 };

enum class Stopbits : uint8_t { ONE = 1, TWO = 2 };

enum class Mode : uint8_t { TX_ONLY = 0, RX_ONLY = 1, TX_RX = 2 };

struct InitData {
  Baudrate baudrate;
  Parity parity;
  Stopbits stopbits;
  Mode mode;
};

Status init(const InitData &init);
Status setBaudrate(Baudrate baudrate);
Status setParity(Parity parity);
Status setStopbits(Stopbits stopbits);
Status setRxCallback(std::function<void(uint8_t)> callback);
Status sendArray(std::span<uint8_t> data);
}  // namespace Uart

Uart::Baudrate operator""_bod(unsigned long long value);

