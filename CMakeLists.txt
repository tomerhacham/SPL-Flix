cmake_minimum_required(VERSION 3.15)
project(assignment_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)

add_executable(assignment_1
        bin/Main.cpp
        include/Action.h
        include/Session.h
        include/User.h
        include/Watchable.h
        json.hpp bin/Action.cpp bin/Session.cpp bin/User.cpp bin/Watchable.cpp)
