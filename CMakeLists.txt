cmake_minimum_required (VERSION 3.12)

project ("wing_ding")

add_executable (wing_ding "src/wing_ding.cpp" "src/wing_ding.h")
set_property(TARGET wing_ding PROPERTY CXX_STANDARD 23)
