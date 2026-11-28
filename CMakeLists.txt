cmake_minimum_required(VERSION 4.2)
project(slipstream)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(MDClient
                src/MDClient.cpp
                src/csvParser.cpp
                include/csvParser.h
                src/MappedFile.cpp
                include/MappedFile.h)


add_executable(slipstream
        include/libslipstream_codec.h
        src/libslipstream_codec.cpp
        src/main.cpp
        include/types.h
        src/OEClient.cpp
        src/MappedFile.cpp
        include/MappedFile.h
        src/orderbook.cpp
        include/orderbook.h
        src/csvParser.cpp
        include/csvParser.h
)
