cmake_minimum_required(VERSION 3.12)
project(synthrax CXX)

# Find the libpcap package
find_package(CURL REQUIRED)
add_executable(synthrax src/main.cpp)

# We want to have the binary compiled in the same folder as the code
set_target_properties("synthrax" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Link the executable with the required libraries
target_link_libraries(synthrax PRIVATE 
CURL::libcurl
)

