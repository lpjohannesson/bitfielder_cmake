cmake_minimum_required(VERSION 3.5)
project(bitfielder)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

