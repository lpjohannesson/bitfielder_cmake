cmake_minimum_required(VERSION 3.5)
project(bitfielder)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(client
    src/core/direction.cpp
    src/core/packet.cpp
    src/core/game_time.cpp

    src/engine/engine.cpp
    src/engine/file_loader.cpp
    src/engine/input.cpp
    src/engine/input_action.cpp

    src/gfx/core/renderer.cpp
    src/gfx/core/shader.cpp
    src/gfx/core/texture.cpp
    src/gfx/core/texture_atlas.cpp

    src/gfx/sprite/sprite_mesh.cpp
    src/gfx/sprite/sprite_program.cpp
    src/gfx/sprite/sprite_renderer.cpp
    src/gfx/sprite/sprite_batch.cpp

    src/gfx/sprite/animation/sprite_frames.cpp
    src/gfx/sprite/animation/sprite_animation.cpp
    src/gfx/sprite/animation/sprite_animation_set.cpp

    src/world/world.cpp
    src/world/content.cpp

    src/world/block/block_map_element.cpp
    src/world/block/block_chunk.cpp
    src/world/block/block_types.cpp

    src/world/entity/entities.cpp

    src/world/entity/systems/body_system.cpp

    src/server/server.cpp
    src/server/block_map_generator.cpp

    src/client/client.cpp
    src/client/client_input.cpp
    src/client/local_client_connection.cpp
    src/client/local_server_connection.cpp
    src/client/remote_server_connection.cpp
    
    src/client/world/client_content.cpp
    src/client/world/world_renderer.cpp
    src/client/world/camera.cpp
    src/client/world/shadow_buffer.cpp

    src/client/world/block/block_map_renderer.cpp
    src/client/world/block/renderers/basic_block_renderer.cpp

    src/client/world/entity/entity_renderer.cpp

    src/client/world/entity/systems/sprite_system.cpp
    src/client/world/entity/systems/sprite_animator_system.cpp
    src/client/world/entity/systems/local_player_system.cpp
    
    src/client/scenes/world_scene.cpp
    src/client/scenes/local_world_scene.cpp
    src/client/scenes/remote_world_scene.cpp

    src/client/main.cpp)

add_executable(server
    src/core/packet.cpp
    src/core/game_time.cpp

    src/world/content.cpp
    src/world/world.cpp

    src/world/block/block_map_element.cpp
    src/world/block/block_chunk.cpp
    src/world/block/block_types.cpp

    src/world/entity/entities.cpp

    src/world/entity/systems/body_system.cpp

    src/server/server.cpp
    src/server/block_map_generator.cpp
    src/server/remote_client_connection.cpp
    src/server/server_host.cpp

    src/server/main.cpp)

add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:client>/assets)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(client PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
target_link_libraries(server PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(client PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

find_package(GLEW REQUIRED)
target_link_libraries(client PRIVATE GLEW::GLEW)

set(EnTT_DIR vcpkg_installed/x64-windows/share/entt)
find_package(EnTT CONFIG REQUIRED)
target_link_libraries(client PRIVATE EnTT::EnTT)
target_link_libraries(server PRIVATE EnTT::EnTT)

set(unofficial-enet_DIR vcpkg_installed/x64-windows/share/unofficial-enet)
find_package(unofficial-enet CONFIG REQUIRED)
target_link_libraries(client PRIVATE unofficial::enet::enet)
target_link_libraries(server PRIVATE unofficial::enet::enet)

target_link_libraries(client PRIVATE ws2_32)
target_link_libraries(server PRIVATE ws2_32)

