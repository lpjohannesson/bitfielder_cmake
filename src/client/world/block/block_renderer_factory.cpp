#include "block_renderer_factory.h"
#include "client/scenes/world_scene.h"
#include <iostream>
#include "core/file_loader.h"
#include "core/color.h"
#include "renderers/block_renderer.h"
#include "renderers/basic_block_renderer.h"
#include "renderers/auto_block_renderer.h"
#include "components/block_renderer_component.h"
#include "components/block_partial_component.h"
#include "components/block_color_component.h"
#include "components/block_sound_component.h"
#include "components/block_particle_component.h"
#include "world/registry/components/registry_name_component.h"

using namespace bf;

TextureSection BlockRendererFactory::getBlockTexture(const rapidjson::Value &value, const WorldScene &scene) {
    return RendererFactory::getTexture("assets/textures/world/blocks/", value, scene);
}

TextureSection BlockRendererFactory::getParticleTexture(const rapidjson::Value &value, const WorldScene &scene) {
    return RendererFactory::getTexture("assets/textures/world/blocks/particles/", value, scene);
}

void BlockRendererFactory::createParticle(const std::string name, entt::entity block, WorldScene &scene) {
    entt::registry &blocksRegistry = scene.world.blocks.registry;

    // Load json
    std::string jsonPath = "assets/renderers/blocks/particles/" + name + ".json";
    rapidjson::Document document;
    
    if (!FileLoader::loadJson(jsonPath, document)) {
        std::cout << "Block particle renderer \"" << jsonPath << "\" could not be parsed." << std::endl;
        return;
    }
    
    BlockParticleComponent &particle = 
        blocksRegistry.emplace<BlockParticleComponent>(block, BlockParticleComponent {});
    
    // Load texture
    TextureSection texture = getParticleTexture(document.GetObject(), scene);

    // Load frames
    int frameCount = document["frameCount"].GetInt();

    particle.frames.loadFrames(texture.uvBox, { frameCount, 1 });
    particle.size = glm::vec2(texture.box.size.x / frameCount, texture.box.size.y) / 16.0f;
}

void BlockRendererFactory::createBlock(entt::entity block, WorldScene &scene) {
    entt::registry &blocksRegistry = scene.world.blocks.registry;
    std::string name = blocksRegistry.get<RegistryNameComponent>(block).name;

    // Load json
    std::string jsonPath = "assets/renderers/blocks/" + name + ".json";
    rapidjson::Document document;
    
    if (!FileLoader::loadJson(jsonPath, document)) {
        std::cout << "Block renderer \"" << jsonPath << "\" could not be parsed." << std::endl;
        return;
    }

    rapidjson::Value &value = document["block"];

    if (value.HasMember("partial")) {
        blocksRegistry.emplace<BlockPartialComponent>(block, BlockPartialComponent {});
    }

    // Create renderer by type
    if (!value.HasMember("type")) {
        return;
    }
    
    BlockRenderer *blockRenderer;

    std::string blockType = value["type"].GetString();

    if (blockType == "basic") {
        blockRenderer = new BasicBlockRenderer(value, block, scene);
    }
    else if (blockType == "auto") {
        blockRenderer = new AutoBlockRenderer(value, block, scene);
    }
    else {
        return;
    }

    blocksRegistry.emplace<BlockRendererComponent>(block, BlockRendererComponent { blockRenderer });

    if (document.HasMember("color")) {
        blocksRegistry.emplace<BlockColorComponent>(block, BlockColorComponent { Color::parseHex(document["color"].GetString()) });
    }

    if (document.HasMember("sound")) {
        SoundSet *soundSet = scene.clientContent.blockSounds.getSoundSet(document["sound"].GetString());
        blocksRegistry.emplace<BlockSoundComponent>(block, BlockSoundComponent { soundSet });
    }

    if (document.HasMember("particle")) {
        createParticle(document["particle"].GetString(), block, scene);
    }
}

void BlockRendererFactory::start(WorldScene &scene) {
    for (entt::entity block : scene.world.blocks.entities) {
        createBlock(block, scene);
    }
}

void BlockRendererFactory::end(WorldScene &scene) {
    entt::registry &blocksRegistry = scene.world.blocks.registry;

    for (entt::entity block : scene.world.blocks.entities) {
        if (!blocksRegistry.all_of<BlockRendererComponent>(block)) {
            continue;
        }

        delete blocksRegistry.get<BlockRendererComponent>(block).renderer;
        blocksRegistry.erase<BlockRendererComponent>(block);
    }
}