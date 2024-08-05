# STM32 UART API module
## Description
This module realizes all UART api functions. It also implements structure to link `UART_HandleTypeDef` handles with rx buffer and rx,tx callbacks. This structure is used to register interface callbacks after initialization function.

## Using
If you use STM32CubeMx you can follow steps:
- Set CMake as Toolchain
- Set Generate peripheral initialization as a pair of '.c/.h' files per peripheral
- Setup used USART peripheral and enable interrupts 
- Uncomment (or define) define directives in `uart_config.hpp` (`U1_ENABLED` compared with `USART1` interface). Because it uses stm32 handles generated, so if you enable not configured in CubeMx interface then module won't compile
- Include `api_uart.hpp` header



