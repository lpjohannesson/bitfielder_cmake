cmake_minimum_required(VERSION 3.0)
project(bitfielder)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(COPY assets DESTINATION ./)

add_executable(bitfielder
    src/engine.cpp
    src/file_loader.cpp
    src/main_scene.cpp

    src/gfx/renderer.cpp
    src/gfx/shader.cpp
    src/gfx/sprite_batch.cpp
    src/gfx/sprite_mesh.cpp
    src/gfx/sprite_renderer.cpp
    src/gfx/texture.cpp
    src/gfx/texture_atlas.cpp

    src/main.cpp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(bitfielder PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(bitfielder PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

find_package(GLEW REQUIRED)
target_link_libraries(bitfielder PRIVATE GLEW::GLEW)

