# STM32 UART API module
## Description
This module realizes all UART api functions. It also implements structure to link `UART_HandleTypeDef` handles with rx buffer and rx, tx callbacks and methods to work with it. Structure is used to register interface callbacks after initialization function to use it from hal callbacks.

## Using
If you use STM32CubeMx you can follow steps:
- Set CMake as project Toolchain, add uart_stm32 library
- Set Generate peripheral initialization as a pair of '.c/.h' files per peripheral to generate `usart.h` file (otherwise you can change `usart.h` include to `main.h` in `uart_stm32.hpp` header)
- Setup used USART peripheral and enable interrupts from it (do not generate init functions or disable functions calls)
- Uncomment (or add) define directives in `uart_config.hpp` (for example, `U1_ENABLED` compared with `USART1` interface, now supported up to 6 interfaces). It uses stm32 handles generated, so if you enable not configured in CubeMx interface then module won't compile
- Include `api_uart.hpp` header





