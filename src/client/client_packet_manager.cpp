#include "client_packet_manager.h"
#include "client/scenes/world_scene.h"
#include "core/packet_types.h"
#include "world/block/block_serializer.h" 
#include "world/entity/components/flip_component.h"
#include "world/entity/components/aim_component.h"
#include "world/entity/components/local_player_component.h"
#include "world/entity/components/sprite_set_component.h"

using namespace bf;

void ClientPacketManager::writePlayerState(WorldScene &scene) {
    entt::entity player = scene.clientContent.player;
    entt::registry &entityRegistry = scene.world.entities.registry;

	Packet packet;

	glm::vec2 position = entityRegistry.get<PositionComponent>(player).position;
	AnimationComponent &animation = entityRegistry.get<AnimationComponent>(player);
	FlipComponent &flip = entityRegistry.get<FlipComponent>(player);
	AimComponent &aim = entityRegistry.get<AimComponent>(player);

	packet << (int)ClientPacket::PLAYER_STATE << position << animation.index << flip.flipX << aim.aim;
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

void ClientPacketManager::readEntityAnimation(Packet &packet, WorldScene &scene) {
    entt::registry &entityRegistry = scene.world.entities.registry;
	entt::entity entity;
	
	if (!readEntityPacket(entity, packet, scene)) {
		return;
	}

	int index;
	packet >> index;

	AnimatorComponent &animator = entityRegistry.get<AnimatorComponent>(entity);
	AnimationComponent &animation = entityRegistry.get<AnimationComponent>(entity);

	AnimatorSystem::playAnimation(animator, animation, index);
}

void ClientPacketManager::readEntityFlip(Packet &packet, WorldScene &scene) {
	entt::registry &entityRegistry = scene.world.entities.registry;
	entt::entity entity;
	
	if (!readEntityPacket(entity, packet, scene)) {
		return;
	}

	FlipComponent &flip = entityRegistry.get<FlipComponent>(entity);
	packet >> flip.flipX;
}

void ClientPacketManager::readEntityAim(Packet &packet, WorldScene &scene) {
	entt::registry &entityRegistry = scene.world.entities.registry;
	entt::entity entity;
	
	if (!readEntityPacket(entity, packet, scene)) {
		return;
	}

	AimComponent &aim = entityRegistry.get<AimComponent>(entity);
	packet >> aim.aim;
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

		case ServerPacket::ENTITY_ANIMATION:
			readEntityAnimation(packet, scene);
			break;

		case ServerPacket::ENTITY_FLIP:
			readEntityFlip(packet, scene);
			break;

		case ServerPacket::ENTITY_AIM:
			readEntityAim(packet, scene);
			break;
		
		case ServerPacket::REMOTE_PLAYER:
			readRemotePlayer(packet, scene);
			break;
		}
	}
}