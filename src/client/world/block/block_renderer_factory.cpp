#include "block_renderer_factory.h"
#include <iostream>
#include "client/scenes/world_scene.h"
#include "core/file_loader.h"
#include "core/color.h"
#include "renderers/basic_block_renderer.h"
#include "renderers/auto_block_renderer.h"
#include "components/block_renderer_component.h"
#include "components/block_particle_component.h"
#include "components/partial_block_component.h"
#include "world/registry/components/registry_name_component.h"
#include "client/world/block/components/block_sound_component.h"

using namespace bf;

TextureSection BlockRendererFactory::getRendererTexture(const WorldScene &scene, const rapidjson::Value &value, std::string basePath) {
    std::string texturePath = value["texture"].GetString();
    std::string textureFullPath = basePath + texturePath + ".png";

    const TextureAtlas &textureAtlas = scene.worldRenderer.textureAtlas;

    return textureAtlas.getSection(textureFullPath);
}

TextureSection BlockRendererFactory::getBlockTexture(const WorldScene &scene, const rapidjson::Value &value) {
    return getRendererTexture(scene, value, "assets/textures/world/blocks/");
}

TextureSection BlockRendererFactory::getParticleTexture(const WorldScene &scene, const rapidjson::Value &value) {
    return getRendererTexture(scene, value, "assets/textures/world/blocks/particles/");
}

void BlockRendererFactory::createParticleRenderer(WorldScene &scene, entt::entity block, const rapidjson::Value &blockValue, std::string particleName) {
    entt::registry &blockRegistry = scene.world.blocks.registry;

    // Load json
    std::string jsonPath = "assets/renderers/blocks/particles/" + particleName + ".json";
    std::string json;
    
    if (!FileLoader::loadText(jsonPath.c_str(), json)) {
        return;
    }

    // Parse json
    rapidjson::Document document;
    
    if (document.Parse(json.c_str()).HasParseError()) {
        std::cout << "Block particle renderer \"" << jsonPath << "\" could not be parsed." << std::endl;
        return;
    }
    
    BlockParticleComponent &blockParticle = 
        blockRegistry.emplace<BlockParticleComponent>(block, BlockParticleComponent {});
    
    // Load texture
    TextureSection texture = getParticleTexture(scene, document.GetObject());

    // Load frames
    int frameCount = document["frameCount"].GetInt();

    blockParticle.frames.loadFrames(texture.uvBox, { frameCount, 1 });

    blockParticle.size = glm::vec2(texture.box.size.x / frameCount, texture.box.size.y) / 16.0f;

    // Load same colour as block
    if (blockValue.HasMember("color")) {
        blockParticle.color = Color::parseHex(blockValue["color"].GetString());
    }
}

BlockRenderer *BlockRendererFactory::createBasicBlockRenderer(const WorldScene &scene, const rapidjson::Value &value) {
    BasicBlockRenderer *blockRenderer = new BasicBlockRenderer();

    blockRenderer->sprite.box.size = glm::vec2(1.0f);
    blockRenderer->sprite.uvBox = getBlockTexture(scene, value).uvBox;

    if (value.HasMember("color")) {
        blockRenderer->sprite.color = Color::parseHex(value["color"].GetString());
    }

    return blockRenderer;
}

BlockRenderer *BlockRendererFactory::createAutoBlockRenderer(const WorldScene &scene, const rapidjson::Value &value) {
    AutoBlockRenderer *blockRenderer = new AutoBlockRenderer();

    blockRenderer->loadFrames(getBlockTexture(scene, value).uvBox);

    if (value.HasMember("color")) {
        blockRenderer->sprite.color = Color::parseHex(value["color"].GetString());
    }

    return blockRenderer;
}

void BlockRendererFactory::createBlockRenderer(WorldScene &scene, entt::entity block) {
    entt::registry &blockRegistry = scene.world.blocks.registry;

    // Get block name
    std::string blockName = blockRegistry.get<RegistryNameComponent>(block).name;

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

    rapidjson::Value &blockValue = document["block"];

    if (blockValue.HasMember("partial")) {
        blockRegistry.emplace<PartialBlockComponent>(block, PartialBlockComponent {});
    }

    // Create particles
    if (document.HasMember("particle")) {
        createParticleRenderer(scene, block, blockValue, document["particle"].GetString());
    }

    // Create renderer by type
    if (!blockValue.HasMember("type")) {
        return;
    }
    
    BlockRenderer *blockRenderer;

    std::string blockType = blockValue["type"].GetString();

    if (blockType == "basic") {
        blockRenderer = createBasicBlockRenderer(scene, blockValue);
    }
    else if (blockType == "auto") {
        blockRenderer = createAutoBlockRenderer(scene, blockValue);
    }
    else {
        return;
    }

    blockRegistry.emplace<BlockRendererComponent>(block, BlockRendererComponent { blockRenderer });

    // Attach sounds
    if (document.HasMember("sound")) {
        SoundSet *soundSet = scene.clientContent.blockSounds.getSoundSet(document["sound"].GetString());

        blockRegistry.emplace<BlockSoundComponent>(block, BlockSoundComponent { soundSet });
    }
}

void BlockRendererFactory::createRenderers(WorldScene &scene) {
    for (entt::entity block : scene.world.blocks.entities) {
        createBlockRenderer(scene, block);
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