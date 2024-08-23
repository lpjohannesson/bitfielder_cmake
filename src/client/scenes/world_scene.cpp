#include "world_scene.h"
#include "engine/engine.h"
#include "core/game_time.h"
#include "sound/sound.h"
#include "menu_scene.h"
#include "world/world_file.h"
#include "client/world/block/components/block_particle_component.h"
#include "client/world/entity/systems/particle_system.h"

using namespace bf;

void WorldScene::updateBlock(glm::ivec2 position) {
	Box2i box{ position - 1, { 2, 2 } };

	world.updateBlock(position, box);

	// Get sample including neighbours
	BlockSample<BlockChunk> blockSample;
	blockSample.sampleBlocks(world.map, box.start.x, box.start.x + box.size.x);

	// Generate section with neighbours
	int sectionStart = glm::max(0, BlockMesh::getSectionIndex(box.start.y));
	int sectionEnd = glm::min(BlockMesh::SECTION_COUNT - 1, BlockMesh::getSectionIndex(box.start.y + box.size.y));

	// Update meshes
	for (BlockChunk *chunk : blockSample.chunks) {
		if (chunk == nullptr) {
			continue;
		}

		worldRenderer.map.createMesh(*chunk, *this, sectionStart, sectionEnd);
	}
}

void WorldScene::spawnBlockParticles(glm::vec2 position, entt::entity block) {
	entt::registry &blocksRegistry = world.blocks.registry;

	if (!blocksRegistry.all_of<BlockParticleComponent>(block)) {
		return;
	}

	BlockParticleComponent blockParticle = blocksRegistry.get<BlockParticleComponent>(block);
	clientContent.particleSystem.spawnParticleExplosion({ blockParticle.frames, position + glm::vec2(0.5f), blockParticle.size }, *this);
}

void WorldScene::placeBlock(int blockIndex, glm::ivec2 position, bool onFrontLayer, BlockData &blockData) {
	if (onFrontLayer) {
		blockData.setFrontIndex(blockIndex);
	}
	else {
		blockData.setBackIndex(blockIndex);
	}

	entt::entity block = world.blocks.getEntity(blockIndex);

	EffectSpriteSystem::spawnEffect(glm::vec2(position) + glm::vec2(0.5f),
		clientContent.placeEffectProperties, *this);

	updateBlock(position);

	client->playRandomPitchSound(clientContent.placeSound);

	BlockSounds::playBlockSound(*this, block);
}

void WorldScene::destroyBlock(glm::ivec2 position, bool onFrontLayer, BlockData &blockData) {
	int blockIndex;

	if (onFrontLayer) {
		blockIndex = blockData.getFrontIndex();
		blockData.setFrontIndex(0);
	}
	else {
		blockIndex = blockData.getBackIndex();
		blockData.setBackIndex(0);
	}

	entt::entity block = world.blocks.getEntity(blockIndex);

	EffectSpriteSystem::spawnEffect(glm::vec2(position) + glm::vec2(0.5f),
		clientContent.destroyEffectProperties, *this);

	spawnBlockParticles(position, block);

	updateBlock(position);

	client->playRandomPitchSound(clientContent.destroySound);
	BlockSounds::playBlockSound(*this, block);
}

bool WorldScene::updatePauseMenu() {
	pauseOptionList.update();
	ListOption *pressedOption = pauseOptionList.getPressedOption();

	if (pressedOption == nullptr) {
		return true;
	}

	if (pressedOption == &pauseContinueOption) {
		paused = false;
		return true;
	}

	if (pressedOption == &pauseTitleOption) {
		MenuScene *menuScene = new MenuScene();
		menuScene->changeState(MenuState::HOME);

		engine->changeScene(menuScene);
		return false;
	}

	return true;
}

void WorldScene::updateSize(glm::ivec2 size) {
	client->updateSize(size);
	worldRenderer.updateSize(size, *this);
}

void WorldScene::update() {
	if (client->clientInput.pause.justPressed()) {
		paused = !paused;
	}

	// Stop in case server ended
	if (!server->host(*this)) {
		return;
	}

	if (paused) {
		// End if pause menu ended
		if (!updatePauseMenu()) {
			return;
		}
	}
	else {
		for (ClientEntitySystem *system : entitySystems) {
			system->update(*this);
		}

		float zoomDirection = 
        (float)client->clientInput.zoomIn.justPressed() -
        (float)client->clientInput.zoomOut.justPressed();
	
		if (zoomDirection != 0.0f) {
			camera.setZoom(glm::max(1.0f, camera.getZoom() + zoomDirection));
			worldRenderer.updateTransforms(*this);
		}
	}

	camera.update(*this);
}

void WorldScene::render() {
	worldRenderer.render(*this);

	if (paused) {
		// Render pause screen
		client->spriteProgram.setTransform(client->getMenuTransform());
		client->spriteRenderer.renderMesh(pauseLogoMesh, client->spriteProgram, worldRenderer.texture);

		pauseOptionList.render();
	}
}

void WorldScene::start() {
	gameTime.reset();

	camera.reset(*this);
	camera.setZoom(3.0f);

	// Create pause screen
	TextureSection pausedLogoTexture = worldRenderer.textureAtlas.getSection("assets/textures/world/paused_logo.png");
	Client::renderLogo(pausedLogoTexture, pauseLogoMesh);

	pauseContinueOption.text = "Continue";
	pauseTitleOption.text = "Return to title";

	pauseOptionList.setOptions({ &pauseContinueOption, &pauseTitleOption });
	pauseOptionList.updateMesh();

	worldRenderer.hud.start(*this);
}

void WorldScene::end() {
	server->end();
	delete server;

	engine->sound.reset();
}

WorldScene::WorldScene() :
	clientContent(*this),
	pauseOptionList(optionListRenderer),
	pauseLogoMesh(client->spriteRenderer) {

}