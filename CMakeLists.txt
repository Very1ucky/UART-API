cmake_minimum_required(VERSION 3.22)

add_library(uart_stm32 INTERFACE)

target_include_directories(uart_stm32 INTERFACE Header)
target_sources(uart_stm32 INTERFACE Modules/STM32/uart_stm32.cpp Modules/Common/utils.cpp)