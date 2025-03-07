cmake_minimum_required(VERSION 3.10)

project(Cathedral)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

set(SOURCE_FILES
    src/main.cpp
    src/gui/main_window.cpp
    src/gui/component_item.cpp
    src/gui/wire_item.cpp
    src/util/logging.cpp
    src/core/circuit.cpp
)

set(HEADER_FILES
    include/gui/main_window.h
    include/gui/component_item.h  # Explicitly include for MOC
    include/gui/wire_item.h
    include/util/logging.h
    include/core/circuit.h
)

add_executable(Cathedral ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Cathedral Qt5::Widgets)

set_target_properties(Cathedral PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(
    TARGET Cathedral POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Build completed. Executable at: ${CMAKE_BINARY_DIR}/bin/Cathedral"
)