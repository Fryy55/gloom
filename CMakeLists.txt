cmake_minimum_required(VERSION 4.0)

project(gloom)

set(CMAKE_CXX_STANDARD 23)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/glad)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${SRC_DIR}/*.cpp
    ${GLAD_DIR}/*.c
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${GLAD_DIR}/include

        ${CMAKE_SOURCE_DIR}/include
)

include(cmake/CPM.cmake)
CPMAddPackage("gh:glfw/glfw#3.4")
target_link_libraries(${PROJECT_NAME} glfw)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<0:>)