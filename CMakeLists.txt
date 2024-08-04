cmake_minimum_required(VERSION 3.22)

add_library(uart_stm32 INTERFACE)

target_include_directories(uart_stm32 INTERFACE 
Header
Modules/STM32/Inc
)
target_sources(uart_stm32 INTERFACE 
    Modules/Common/utils.cpp
    Modules/STM32/Src/uart_stm32.cpp 
    Modules/STM32/Src/uart_callback.cpp
)