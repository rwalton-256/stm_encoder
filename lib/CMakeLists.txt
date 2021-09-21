cmake_minimum_required (VERSION 2.8.11)

set( FreeRTOS_DIR "FreeRTOS/FreeRTOS/Source" )

add_library(
    FreeRTOS_lib
    ${FreeRTOS_DIR}/croutine.c
    ${FreeRTOS_DIR}/event_groups.c
    ${FreeRTOS_DIR}/list.c
    ${FreeRTOS_DIR}/queue.c
    ${FreeRTOS_DIR}/stream_buffer.c
    ${FreeRTOS_DIR}/tasks.c
    ${FreeRTOS_DIR}/timers.c
    ${FreeRTOS_DIR}/portable/MemMang/heap_5.c
    ${FreeRTOS_DIR}/portable/GCC/ARM_CM0/port.c
)

target_include_directories(
    FreeRTOS_lib PUBLIC
    ${FreeRTOS_DIR}/include
    ${FreeRTOS_DIR}/portable/GCC/ARM_CM0
    config/include/FreeRTOS
)

FILE(GLOB STM32f0xx_hal_srcs STM32CubeF0/Drivers/STM32F0xx_HAL_Driver/src/*.c)

add_library(
    STM32f0xx_hal_lib
    ${STM32f0xx_hal_srcs}
    STM32CubeF0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/system_stm32f0xx.c
    STM32CubeF0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f042x6.s
)

target_include_directories(
    STM32f0xx_hal_lib PUBLIC
    STM32CubeF0/Drivers/STM32F0xx_HAL_Driver/Inc
    config/include/STM32CubeF0
    STM32CubeF0/Drivers/CMSIS/Device/ST/STM32F0xx/Include
    STM32CubeF0/Drivers/CMSIS/Include
)

target_compile_definitions(
    STM32f0xx_hal_lib PUBLIC
    STM32F042x6
)
