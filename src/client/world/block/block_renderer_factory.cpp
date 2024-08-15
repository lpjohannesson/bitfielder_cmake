#include "block_renderer_factory.h"
#include <iostream>
#include "client/scenes/world_scene.h"
#include "core/file_loader.h"
#include "core/color.h"
#include "renderers/basic_block_renderer.h"
#include "renderers/auto_block_renderer.h"
#include "components/block_renderer_component.h"
#include "components/block_partial_component.h"
#include "components/block_sprite_component.h"
#include "world/registry/components/registry_name_component.h"
#include "client/world/block/components/block_sound_component.h"

using namespace bf;

TextureSection BlockRendererFactory::getTexture(std::string basePath, const rapidjson::Value &value, const WorldScene &scene) {
    std::string texturePath = value["texture"].GetString();
    std::string textureFullPath = basePath + texturePath + ".png";

    const TextureAtlas &textureAtlas = scene.worldRenderer.textureAtlas;

    return textureAtlas.getSection(textureFullPath);
}

TextureSection BlockRendererFactory::getBlockTexture(const rapidjson::Value &value, const WorldScene &scene) {
    return getTexture("assets/textures/world/blocks/", value, scene);
}

TextureSection BlockRendererFactory::getParticleTexture(const rapidjson::Value &value, const WorldScene &scene) {
    return getTexture("assets/textures/world/blocks/particles/", value, scene);
}

BlockParticleComponent *BlockRendererFactory::createParticleRenderer(const std::string name, entt::entity block, WorldScene &scene) {
    entt::registry &blockRegistry = scene.world.blocks.registry;

    // Load json
    std::string jsonPath = "assets/renderers/blocks/particles/" + name + ".json";
    std::string json;
    
    if (!FileLoader::loadText(jsonPath.c_str(), json)) {
        return nullptr;
    }

    // Parse json
    rapidjson::Document document;
    
    if (document.Parse(json.c_str()).HasParseError()) {
        std::cout << "Block particle renderer \"" << jsonPath << "\" could not be parsed." << std::endl;
        return nullptr;
    }
    
    BlockParticleComponent &particle = 
        blockRegistry.emplace<BlockParticleComponent>(block, BlockParticleComponent {});
    
    // Load texture
    TextureSection texture = getParticleTexture(document.GetObject(), scene);

    // Load frames
    int frameCount = document["frameCount"].GetInt();

    particle.frames.loadFrames(texture.uvBox, { frameCount, 1 });
    particle.size = glm::vec2(texture.box.size.x / frameCount, texture.box.size.y) / 16.0f;

    return &particle;
}

void BlockRendererFactory::createBlockRenderer(entt::entity block, WorldScene &scene) {
    entt::registry &blocksRegistry = scene.world.blocks.registry;
    std::string blockName = blocksRegistry.get<RegistryNameComponent>(block).name;

    // Load json
    std::string jsonPath = "assets/renderers/blocks/" + blockName + ".json";
    std::string json;
    
    if (!FileLoader::loadText(jsonPath.c_str(), json)) {
        return;
    }

    // Parse json
    rapidjson::Document document;
    
    if (document.Parse(json.c_str()).HasParseError()) {
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

    // Set colour
    glm::vec4 color;

    if (document.HasMember("color")) {
        color = Color::parseHex(document["color"].GetString());
    }
    else {
        color = glm::vec4(1.0f);
    }

    if (blocksRegistry.all_of<BlockSpriteComponent>(block)) {
        Sprite &sprite = blocksRegistry.get<BlockSpriteComponent>(block).sprite;
        sprite.color = color;
    }

    // Create particles
    if (document.HasMember("particle")) {
        BlockParticleComponent *particle = createParticleRenderer(document["particle"].GetString(), block, scene);

        if (particle != nullptr) {
            particle->color = color;
        }
    }

    // Attach sounds
    if (document.HasMember("sound")) {
        SoundSet *soundSet = scene.clientContent.blockSounds.getSoundSet(document["sound"].GetString());

        blocksRegistry.emplace<BlockSoundComponent>(block, BlockSoundComponent { soundSet });
    }
}

void BlockRendererFactory::createRenderers(WorldScene &scene) {
    for (entt::entity block : scene.world.blocks.entities) {
        createBlockRenderer(block, scene);
    }
}

void BlockRendererFactory::destroyRenderers(WorldScene &scene) {
    entt::registry &blockRegistry = scene.world.blocks.registry;

    for (entt::entity block : scene.world.blocks.entities) {
        // Check block has renderer
        if (!blockRegistry.all_of<BlockRendererComponent>(block)) {
            continue;
        }

        delete blockRegistry.get<BlockRendererComponent>(block).renderer;
        blockRegistry.erase<BlockRendererComponent>(block);
    }
}