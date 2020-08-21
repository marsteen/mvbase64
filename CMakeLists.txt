cmake_minimum_required(VERSION 3.10)
project("base64")

include_directories(./)

# strip .exe
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

add_executable(base64
    main.cpp
    CBase64.cpp
)


