cmake_minimum_required (VERSION 3.12)

project ("wing_ding")

add_subdirectory ("vendor")

file (GLOB SOURCE_FILES
    "src/*.h"
    "src/*/*.h"
    "src/*/*/*.h"
    "src/*.cpp"
    "src/*/*.cpp"
    "src/*/*/*.cpp"
)

if(WIN32 AND NO_CONSOLE)
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SOURCE_FILES})
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE NO_CONSOLE_WINDOW)
else()
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
endif()
set_property (TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_include_directories (${CMAKE_PROJECT_NAME} PRIVATE "vendor/glfw/include/" "vendor/glad-cmake/include" "vendor/glm/glm/")
target_link_libraries (${CMAKE_PROJECT_NAME} PRIVATE "glfw" "glad")