cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(pmw3901-driver)

zephyr_library()
zephyr_library_sources(
    src/pwm3901.c
)
zephyr_library_include_directories(
    include
)