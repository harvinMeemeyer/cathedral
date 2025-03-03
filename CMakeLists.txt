cmake_minimum_required(VERSION 3.16)
project(Cathedral)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directory for compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find Qt
set(Qt5_DIR "/opt/homebrew/opt/qt/lib/cmake/Qt5")  # Adjust based on `brew --prefix qt`
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Add sources
set(SOURCES
    src/main.cpp
    src/gui/main_window.cpp
    src/gui/component_item.cpp
    src/util/logging.cpp
    src/core/circuit.cpp
)

set(HEADERS
    include/gui/main_window.h
)

# ✅ Tell CMake to process Qt's Q_OBJECT macros
qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

# Include directories
include_directories(include)

# Create executable
add_executable(Cathedral ${SOURCES} ${MOC_SOURCES})  # ✅ Add MOC-generated sources
target_link_libraries(Cathedral Qt5::Widgets)

# Enable debugging symbols
set(CMAKE_BUILD_TYPE Debug)
