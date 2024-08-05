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
/**
 * @brief Initializes uart interface. Doesn't start receiving (use
 * startReceiving function to do it)
 *
 * @param interface : used interface
 * @param params : init paramaters
 * @return Status : if parameters aren't valid, interface is already initialized
 * or initialization failed, returns Status::ERROR, otherwise returns Status::OK
 */
Status init(Interface interface, const Params &params);
/**
 * @brief Deinitializes uart interface
 *
 * @param interface : used interface
 * @return Status : if deinitialization failed or interface wasn't initialized,
 * returns Status::ERROR, otherwise returns Status::OK
 */
Status deinit(Interface interface);
/**
 * @brief Changes interface baudrate. Resumes receiving with buffer wipe (if was
 * started), doesn't resumes transmiting
 *
 * @param interface : used interface
 * @param baudrate : new baudrate value
 * @return Status : if change failed or interface wasn't initialized, returns
 * Status::ERROR, otherwise returns Status::OK
 */
Status changeBaudrate(Interface interface, Baudrate baudrate);
/**
 * @brief Changes interface parity. Resumes receiving with buffer wipe (if was
 * started), doesn't resumes transmiting
 *
 * @param interface : used interface
 * @param parity : new parity value
 * @return Status : if change failed or interface wasn't initialized, returns
 * Status::ERROR, otherwise returns Status::OK
 */
Status changeParity(Interface interface, Parity parity);
/**
 * @brief Changes interface stopbits. Resumes receiving with buffer wipe (if was
 * started), doesn't resumes transmiting
 *
 * @param interface : used interface
 * @param stopbits : new stopbits value
 * @return Status : if change failed or interface wasn't initialized, returns
 * Status::ERROR, otherwise returns Status::OK
 */
Status changeStopbits(Interface interface, Stopbits stopbits);
/**
 * @brief Changes interface received callback. Resumes receiving with buffer
 * wipe (if was started), doesn't resumes transmiting
 *
 * @param interface : used interface
 * @param callback : new received callback
 * @return Status : if change failed or interface wasn't initialized, returns
 * Status::ERROR, otherwise returns Status::OK
 */
Status changeReceivedCallback(Interface interface,
                              std::function<void(std::span<uint8_t>)> callback);
/**
 * @brief Changes interface transmit callback. Resumes receiving with buffer
 * wipe (if was started), doesn't resumes transmiting
 *
 * @param interface : used interface
 * @param callback : new transmit callback
 * @return Status : if change failed or interface wasn't initialized, returns
 * Status::ERROR, otherwise returns Status::OK
 */
Status changeTransmitCompletedCallback(Interface interface,
                                       std::function<void()> callback);
/**
 * @brief Starts receiving data from uart interface
 *
 * @param interface : used interface
 * @return Status : if receive already started, can't be started or interface
 * wasn't initialized, returns Status::ERROR, otherwise returns Status::OK
 */
Status startReceiving(Interface interface);
/**
 * @brief Stops receiving data from uart interface
 *
 * @param interface : used interface
 * @return Status : if interface wasn't initialized, returns Status::ERROR,
 * otherwise returns Status::OK
 */
Status stopReceiving(Interface interface);
/**
 * @brief Sends bytes via uart interface
 *
 * @param interface : used interface
 * @param data
 * @return Status : if transmit already started, can't be started or interface
 * wasn't initialized, returns Status::ERROR, otherwise returns Status::OK
 */
Status sendBytes(Interface interface, std::span<uint8_t> data);
}  // namespace Uart
/**
 * @brief Literal to get Uart::Baudrate from unsigned long long
 *
 * @param value : baudrate num value
 * @return Uart::Baudrate : B_0 if value can't be converted, otherwise returns
 * other values from Uart::Baudrate enum
 */
Uart::Baudrate operator""_bod(unsigned long long value);
