cmake_minimum_required(VERSION 3.2)
project(ModbusServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

INCLUDE(FindPkgConfig)
pkg_check_modules(MODBUS REQUIRED libmodbus)
include_directories(${MODBUS_INCLUDE_DIRS})
link_directories(${MODBUS_LIBRARY_DIRS})

set(SOURCE_FILES main.cpp)
add_executable(ModbusServer ${SOURCE_FILES} modbusServer.cpp modbusServer.h)

target_link_libraries(ModbusServer ${MODBUS_LIBRARIES} pthread)