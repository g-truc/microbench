set(NAME glu)

file(GLOB ROOT_SOURCE *.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(${NAME} STATIC ${ROOT_SOURCE})
