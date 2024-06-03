cmake_minimum_required(VERSION 3.28)
project(AutoClicker)

set(CMAKE_CXX_STANDARD 20)

add_library(AutoClicker SHARED
        dllMain.cpp
        source/me/sleepyfish/autoclicker/ClickUtils.h
        source/me/sleepyfish/autoclicker/Utils.h
)

target_link_libraries(AutoClicker PRIVATE
        winmm
)