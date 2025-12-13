cmake_minimum_required(VERSION 3.12)
project(synthrax CXX)

# Find the libpcap package
find_package(PCAP REQUIRED)
find_package(Boost REQUIRED COMPONENTS System Filesystem)
add_executable(synthrax main.cpp)

# Link the executable with the required libraries
target_link_libraries(synthrax PRIVATE 
${PCAP_LIBRARIES}
${Boost_LIBRARIES}
)

