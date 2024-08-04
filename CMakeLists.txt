cmake_minimum_required(VERSION 3.10)
project(bitfielder LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

if(WIN32)
    set(CLIENT 1)
    set(SERVER 1)

elseif(NX)
    set(CLIENT 1)
    
endif()

if (CLIENT)
    add_executable(client
        src/core/direction.cpp
        src/core/color.cpp
        src/core/packet.cpp
        src/core/game_time.cpp
        src/core/file_loader.cpp

        src/engine/engine.cpp
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

        src/gfx/text/font.cpp

        src/sound/sound.cpp
        src/sound/sound_set.cpp

        src/world/world.cpp
        src/world/world_file.cpp
        src/world/content.cpp

        src/world/block/block_map_element.cpp
        src/world/block/block_chunk.cpp
        src/world/block/block_types.cpp
        src/world/block/block_light_generator.cpp
        src/world/block/block_serializer.cpp

        src/world/entity/entities.cpp

        src/world/entity/systems/body_system.cpp

        src/server/server.cpp
        src/server/block_map_generator.cpp

        src/client/client.cpp
        src/client/client_input.cpp
        src/client/local_client_connection.cpp
        src/client/local_server_connection.cpp
        src/client/remote_server_connection.cpp
        src/client/font_factory.cpp

        src/client/menu/option_list.cpp
        src/client/menu/option_list_renderer.cpp

        src/client/menu/text_list_option.cpp
        src/client/menu/input_list_option.cpp
        
        src/client/world/client_content.cpp
        src/client/world/world_renderer.cpp
        src/client/world/camera.cpp
        src/client/world/shadow_buffer.cpp

        src/client/world/block/block_mesh.cpp
        src/client/world/block/block_map_renderer.cpp
        src/client/world/block/block_renderer_factory.cpp
        src/client/world/block/block_sounds.cpp

        src/client/world/block/renderers/basic_block_renderer.cpp
        src/client/world/block/renderers/auto_block_renderer.cpp

        src/client/world/entity/entity_renderer.cpp

        src/client/world/entity/systems/client_entity_system.cpp
        src/client/world/entity/systems/sprite_system.cpp
        src/client/world/entity/systems/sprite_animator_system.cpp
        src/client/world/entity/systems/sprite_aim_system.cpp
        src/client/world/entity/systems/particle_system.cpp
        src/client/world/entity/systems/effect_sprite_system.cpp
        src/client/world/entity/systems/local_player_system.cpp
        
        src/client/scenes/menu_scene.cpp
        src/client/scenes/world_scene.cpp

        src/client/main.cpp)
    
    set_target_properties(client
        PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR})
endif()

if (SERVER)
    add_executable(server
        src/core/packet.cpp
        src/core/game_time.cpp
        src/core/file_loader.cpp

        src/world/world.cpp
        src/world/world_file.cpp
        src/world/content.cpp

        src/world/block/block_map_element.cpp
        src/world/block/block_chunk.cpp
        src/world/block/block_types.cpp
        src/world/block/block_light_generator.cpp
        src/world/block/block_serializer.cpp

        src/world/entity/entities.cpp

        src/world/entity/systems/body_system.cpp

        src/server/server.cpp
        src/server/block_map_generator.cpp
        src/server/remote_client_connection.cpp
        src/server/server_host.cpp

        src/server/main.cpp)
endif()

if(WIN32)
    target_compile_definitions(client PUBLIC NOMINMAX)

    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/share)

    find_package(SDL2 CONFIG REQUIRED)
    find_package(EnTT CONFIG REQUIRED)
    find_package(unofficial-enet CONFIG REQUIRED)
    find_package(RapidJSON CONFIG REQUIRED)
    find_package(ZLIB REQUIRED)

    if (CLIENT)
        find_package(PNG REQUIRED)
        find_package(SDL2_image CONFIG REQUIRED)
        find_package(glad CONFIG REQUIRED)
        find_package(OpenAL CONFIG REQUIRED)

        target_compile_definitions(client PRIVATE GLM_ENABLE_EXPERIMENTAL)

        target_link_libraries(client PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
        target_link_libraries(client PRIVATE EnTT::EnTT)
        target_link_libraries(client PRIVATE unofficial::enet::enet)
        target_link_libraries(client PRIVATE rapidjson)
        target_link_libraries(client PRIVATE ZLIB::ZLIB)

        target_link_libraries(client PRIVATE PNG::PNG)
        target_link_libraries(client PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
        target_link_libraries(client PRIVATE glad::glad)
        target_link_libraries(client PRIVATE OpenAL::OpenAL)

        if(WIN32)
            target_link_libraries(client PRIVATE ws2_32)
        endif()
    endif()

    if (SERVER)
        target_compile_definitions(server PRIVATE GLM_ENABLE_EXPERIMENTAL)

        target_link_libraries(server PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
        target_link_libraries(server PRIVATE EnTT::EnTT)
        target_link_libraries(server PRIVATE unofficial::enet::enet)
        target_link_libraries(server PRIVATE rapidjson)
        target_link_libraries(server PRIVATE ZLIB::ZLIB)

        if(WIN32)
            target_link_libraries(server PRIVATE ws2_32)
        endif()
    endif()

elseif(NX)
    target_compile_definitions(client PUBLIC NX)

    set_target_properties(client PROPERTIES
        APP_TITLE "bitfielder"
        APP_AUTHOR "crossfrog"
        APP_VERSION "0.1.0")

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include)

    file(GLOB SWITCH_LIBS_LIST ${SWITCH_LIBS}/*.a)
    list(JOIN SWITCH_LIBS_LIST "\;" SWITCH_LIBS_LIST)
    target_link_libraries(client ${SWITCH_LIBS_LIST})

    target_link_libraries(client
        ${SWITCH_LIBS}/libEGL.a
        ${SWITCH_LIBS}/libdrm_nouveau.a
        ${SWITCH_LIBS}/libglapi.a
        ${LIBNX_LIBS}/libnx.a)

    add_nro_target(client)
endif()

add_custom_target(copy-assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    DEPENDS client)