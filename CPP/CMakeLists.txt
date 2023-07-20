cmake_minimum_required(VERSION 3.25)
project(CPP)

set(CMAKE_CXX_STANDARD 23)

# Set the path to the OpenCV installation
# Make sure to replace /path/to/opencv with the actual path on your system
set(OpenCV_DIR "C:/Users/varun/Downloads/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(C:/Users/varun/Downloads/opencv/build/include)

# Create the executable target
add_executable(CPP main.cpp licence.h)

# Add the static linker options
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(CPP PRIVATE -static-libgcc -static-libstdc++)
endif()

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Link OpenCV libraries to your target
target_link_libraries(CPP ${OpenCV_LIBS})