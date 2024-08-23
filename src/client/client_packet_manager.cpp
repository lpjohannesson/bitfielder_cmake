#include "client_packet_manager.h"
#include "client/scenes/world_scene.h"
#include "core/packet_types.h"
#include "world/block/block_serializer.h" 
#include "world/entity/components/sprite_flip_component.h"
#include "world/entity/components/aim_component.h"
#include "client/world/entity/components/local_player_component.h"
#include "client/world/entity/components/sprite_aim_component.h"
#include "client/world/entity/systems/sprite_aim_system.h"

using namespace bf;

void ClientPacketManager::writePlayerState(WorldScene &scene) {
    entt::entity player = scene.clientContent.player;
    entt::registry &entityRegistry = scene.world.entities.registry;

	Packet packet;

	glm::vec2 position = entityRegistry.get<PositionComponent>(player).position;
	SpriteAnimationComponent &animation = entityRegistry.get<SpriteAnimationComponent>(player);
	SpriteFlipComponent &spriteFlip = entityRegistry.get<SpriteFlipComponent>(player);
	AimComponent &aim = entityRegistry.get<AimComponent>(player);

	packet << (int)ClientPacket::PLAYER_STATE << position << animation.animationIndex << spriteFlip.flipX << aim.aim;
	scene.server->writePacket(packet);
}

void ClientPacketManager::writeReplaceBlock(glm::ivec2 position, bool onFrontLayer, BlockData &blockData, WorldScene &scene) {
	Packet packet;

	packet << (int)ClientPacket::REPLACE_BLOCK << position << onFrontLayer;

	if (onFrontLayer) {
		packet << (int)blockData.getFrontIndex();
	}
	else {
		packet << (int)blockData.getBackIndex();
	}

	scene.server->writePacket(packet);
}

bool ClientPacketManager::readEntityPacket(entt::entity &entity, Packet &packet, WorldScene &scene) {
	int entityID;
	packet >> entityID;
	
	if (!scene.world.entities.getEntity(entityID, entity)) {
		return false;
	}

	return true;
}

void ClientPacketManager::readTeleportPlayer(Packet &packet, WorldScene &scene) {
	glm::vec2 position;
	packet >> position;

	entt::registry &entityRegistry = scene.world.entities.registry;
	entityRegistry.get<PositionComponent>(scene.clientContent.player).position = position;

	scene.camera.reset(scene);
}

void ClientPacketManager::readChunkData(Packet &packet, WorldScene &scene) {
    BlockMap<BlockChunk> &map = scene.world.map;
    BlockMapRenderer &mapRenderer = scene.worldRenderer.map;

	int chunkIndex;
	packet >> chunkIndex;

	// Chunk data
	BlockChunk &chunk = map.createChunk(chunkIndex);
	BlockSerializer::readChunk(packet, chunk, scene.world);

	// Create mesh
	scene.worldRenderer.map.createMesh(chunk, scene);

	// Update neighbour meshes
	BlockChunk
		*leftChunk = map.getChunk(chunkIndex - 1),
		*rightChunk = map.getChunk(chunkIndex + 1);
	
	if (leftChunk != nullptr) {
		mapRenderer.createMesh(*leftChunk, scene);
	}

	if (rightChunk != nullptr) {
		mapRenderer.createMesh(*rightChunk, scene);
	}
}

void ClientPacketManager::readReplaceBlock(Packet &packet, WorldScene &scene) {
	glm::ivec2 position;
	bool onFrontLayer;
	int blockIndex;

	packet >> position >> onFrontLayer >> blockIndex;

	BlockData *blockData = BlockChunk::getWorldBlock(position, scene.world.map);

	if (blockData == nullptr) {
		return;
	}

	if (blockIndex == 0) {
		scene.destroyBlock(position, onFrontLayer, *blockData);
	}
	else {
		scene.placeBlock(blockIndex, position, onFrontLayer, *blockData);
	}
}

void ClientPacketManager::readDespawnEntity(Packet &packet, WorldScene &scene) {
	int entityID;
	packet >> entityID;

	scene.world.entities.despawnEntity(entityID);
}

void ClientPacketManager::readEntityPosition(Packet &packet, WorldScene &scene) {
    entt::registry &entityRegistry = scene.world.entities.registry;
	entt::entity entity;
	
	if (!readEntityPacket(entity, packet, scene)) {
		return;
	}

	PositionComponent &position = entityRegistry.get<PositionComponent>(entity);
	packet >> position.position;
}

void ClientPacketManager::readEntitySpriteAnimation(Packet &packet, WorldScene &scene) {
    entt::registry &entityRegistry = scene.world.entities.registry;
	entt::entity entity;
	
	if (!readEntityPacket(entity, packet, scene)) {
		return;
	}

	int animationIndex;
	packet >> animationIndex;

	SpriteAnimationComponent &animation = entityRegistry.get<SpriteAnimationComponent>(entity);
	SpriteAnimatorComponent &animator = entityRegistry.get<SpriteAnimatorComponent>(entity);

	SpriteAnimatorSystem::playAnimation(animator, animation, animationIndex);
}

void ClientPacketManager::readEntitySpriteFlip(Packet &packet, WorldScene &scene) {
	entt::registry &entityRegistry = scene.world.entities.registry;
	entt::entity entity;
	
	if (!readEntityPacket(entity, packet, scene)) {
		return;
	}

	SpriteFlipComponent &spriteFlip = entityRegistry.get<SpriteFlipComponent>(entity);
	packet >> spriteFlip.flipX;
}

void ClientPacketManager::readEntitySpriteAim(Packet &packet, WorldScene &scene) {
	entt::registry &entityRegistry = scene.world.entities.registry;
	entt::entity entity;
	
	if (!readEntityPacket(entity, packet, scene)) {
		return;
	}

	AimComponent &aim = entityRegistry.get<AimComponent>(entity);
	SpriteAimComponent &spriteAim = entityRegistry.get<SpriteAimComponent>(entity);
	SpriteAnimatorComponent &animator = entityRegistry.get<SpriteAnimatorComponent>(entity);

	packet >> aim.aim;

	animator.frames = SpriteAimSystem::getAimFrames(spriteAim, aim.aim);
}

void ClientPacketManager::readRemotePlayer(Packet &packet, WorldScene &scene) {
	int playerID;

	packet >> playerID;

	// Spawn player entity
	entt::entity player = scene.world.entities.spawnEntity(playerID);
	scene.clientContent.createPlayer(player, scene);
}

void ClientPacketManager::readPacket(Packet &packet, WorldScene &scene) {
	while (!packet.ended()) {
		int packetID;
		packet >> packetID;

		switch ((ServerPacket)packetID) {
		case ServerPacket::TELEPORT_PLAYER:
			readTeleportPlayer(packet, scene);
			break;

		case ServerPacket::CHUNK_DATA:
			readChunkData(packet, scene);
			break;

		case ServerPacket::REPLACE_BLOCK:
			readReplaceBlock(packet, scene);
			break;
		
		case ServerPacket::DESPAWN_ENTITY:
			readDespawnEntity(packet, scene);
			break;
		
		case ServerPacket::ENTITY_POSITION:
			readEntityPosition(packet, scene);
			break;

		case ServerPacket::ENTITY_SPRITE_ANIMATION:
			readEntitySpriteAnimation(packet, scene);
			break;

		case ServerPacket::ENTITY_SPRITE_FLIP:
			readEntitySpriteFlip(packet, scene);
			break;

		case ServerPacket::ENTITY_SPRITE_AIM:
			readEntitySpriteAim(packet, scene);
			break;
		
		case ServerPacket::REMOTE_PLAYER:
			readRemotePlayer(packet, scene);
			break;
		}
	}
}