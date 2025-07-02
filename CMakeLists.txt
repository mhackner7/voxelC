cmake_minimum_required(VERSION 3.10)
project(craftmine C)

add_compile_options(-Wall)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_path(CGLM_INCLUDE_DIR NAMES cglm/cglm.h PATHS /usr/include /usr/local/include)

find_library(CGLM_LIBRARY NAMES cglm PATHS /usr/lib /usr/local/lib)





include_directories(include)

set(CMAKE_C_STANDARD 99)

add_executable(craftmine
    src/main.c
    src/block.c
    src/window.c
    src/glad.c
    src/texture.c
    src/input.c
    src/camera.c
    src/chunk.c
    src/world.c
)

add_custom_target(run
    COMMAND craftmine
    DEPENDS craftmine
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(rerun
    COMMAND ${CMAKE_COMMAND} --build . --target craftmine
    COMMAND craftmine
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)


target_link_libraries(craftmine ${SDL2_LIBRARIES})

if(CGLM_INCLUDE_DIR AND CGLM_LIBRARY)
    message(STATUS "Found cglm includes at ${CGLM_INCLUDE_DIR}")
    message(STATUS "Found cglm library at ${CGLM_LIBRARY}")
    include_directories(${CGLM_INCLUDE_DIR})
    target_link_libraries(craftmine ${CGLM_LIBRARY})
else()
    message(FATAL_ERROR "Could not find cglm")
endif()