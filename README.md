# UART API 
## Description
This project contains platform-independent api to work with uart interfaces written in C++.

## Using
You need at least C++20 standart to use this api.

You can use one of the api implementation from `Modules` folder or write your own following api methods contracts. All modules can use `Modules/Common` source files which help to work with api.

Implemented modules can be easily added as CMake libraries (see `Modules/CMakeLists.txt`).

## Testing
To test your api realization you can use tests from `Tests` folder. Running environment:
- `python`
- `pyserail`
- `pytest`

Following command can be used to run test script:
`pytest -s (script name)`

For using `integration_tests.py` your device should be configured properly. Possible configuration using proposed api can be as follows:
```cpp
  Uart::init(Uart::Interface::U1, Uart::Params{9600_bod,
                                               Uart::Parity::NONE,
                                               Uart::Stopbits::ONE,
                                               Uart::Mode::RX_ONLY,
                                               128,
                                               [](std::span<uint8_t> data) {
                                                 Uart::sendBytes(
                                                     Uart::Interface::U2, data);
                                               },
                                               {}});
  Uart::init(Uart::Interface::U2, Uart::Params{115200_bod,
                                               Uart::Parity::NONE,
                                               Uart::Stopbits::ONE,
                                               Uart::Mode::TX_ONLY,
                                               0,
                                               {},
                                               {}});

  Uart::startReceiving(Uart::Interface::U1);
```



