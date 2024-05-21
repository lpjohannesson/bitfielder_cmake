cmake_minimum_required(VERSION 3.5)
project(bitfielder)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_compile_definitions(BF_CLIENT)

add_executable(${PROJECT_NAME}
    src/engine/engine.cpp
    src/engine/file_loader.cpp

    src/gfx/core/renderer.cpp
    src/gfx/core/shader.cpp
    src/gfx/core/texture.cpp
    src/gfx/core/texture_atlas.cpp
    
    src/gfx/sprite/sprite_batch.cpp
    src/gfx/sprite/sprite_mesh.cpp
    src/gfx/sprite/sprite_renderer.cpp

    src/world/world.cpp

    src/world/block/block_chunk.cpp
    src/world/block/block_map.cpp
    src/world/block/block_types.cpp

    src/client/world/world_renderer.cpp
    src/client/world/block/block_map_renderer.cpp
    src/client/world/block/renderers/basic_block_renderer.cpp

    src/world/content/content.cpp
    src/client/world/content/client_content.cpp

    src/main_scene.cpp

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

set(EnTT_DIR vcpkg_installed/x64-windows/share/entt)
find_package(EnTT CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)

