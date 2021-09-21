cmake_minimum_required (VERSION 2.8.11)

# Name project
project(stm_encoder)

# Prevents cmake from trying to test compiler with test program.
# This is cross compiled so the exectuable obviously won't run.
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
# Set compiler to arm gcc compiler
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
# Prevent dynamic linking? idk
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
# Tell compiler what cpu is in use
add_compile_options(
    -mcpu=cortex-m0
    -fdata-sections
    -ffunction-sections
    -Os
)
# Allows us to comile .s files
enable_language(ASM)

# Add libraries to cmake project
add_subdirectory(lib)

# Create elf file
add_executable(
    ${CMAKE_PROJECT_NAME}
    app/src/angle_poll.c
    app/src/app.c
    app/src/can_wrapper.c
    app/src/encoder_com.c
    app/src/main.c
)

# Set proper name and extension for executable
set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME "${CMAKE_PROJECT_NAME}"
        SUFFIX ".elf"
)

# Link the proper libraries
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    FreeRTOS_lib
    STM32f0xx_hal_lib
)

set_target_properties(
    ${CMAKE_PROJECT_NAME} PROPERTIES LINK_DEPENDS
    STM32F042F4PX_FLASH.ld
)

# Include local include directories
target_include_directories(
    ${CMAKE_PROJECT_NAME} PRIVATE
    app/include/stm_encoder
)

#
target_link_options(
    ${CMAKE_PROJECT_NAME} PUBLIC
    --specs=nosys.specs
    "-Os"
    "-Wl,-Map=${CMAKE_PROJECT_NAME}.map"
    "-Wl,--gc-sections"
    "-TSTM32F042F4PX_FLASH.ld"
)

# Print size of executable
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-size --format=berkeley "${CMAKE_PROJECT_NAME}.elf"
)
