#include "Game.h"

GameManager::GameManager(Server* server) {
	_server = server;
	playerBlue = new Player(PLAYER_FACTION_BLUE);
	playerOrange = new Player(PLAYER_FACTION_ORANGE);
	playerBlue->setEnemy(playerOrange);
	playerOrange->setEnemy(playerBlue);
	aiBlue = new AI(playerBlue);
	aiOrange = new AI(playerOrange);
	playerBluePeer = -1;
	playerOrangePeer = -1;
}

GameManager::~GameManager() {
	delete aiBlue, aiOrange;
	delete playerBlue;
	delete playerOrange;
}

void GameManager::broadcastPlayerPositions(Player* player, PlayerFaction faction, int playerId) {
	PlayerPosition pp;
	pp.playerId = playerId;
	pp.factionId = faction;
	player->getHeadPosition().getSeparateValues(pp.headPosX, pp.headPosY, pp.headPosZ);
	player->getHeadRotation().getValueAsQuat(pp.headRotX, pp.headRotY, pp.headRotZ, pp.headRotW);
	player->getDiskArmPosition().getSeparateValues(pp.rightPosX, pp.rightPosY, pp.rightPosZ);
	player->getDiskArmRotation().getValueAsQuat(pp.rightRotX, pp.rightRotY, pp.rightRotZ, pp.rightRotW);
	player->getShieldArmPosition().getSeparateValues(pp.leftPosX, pp.leftPosY, pp.leftPosZ);
	player->getShieldArmRotation().getValueAsQuat(pp.leftRotX, pp.leftRotY, pp.leftRotZ, pp.leftRotW);
	_server->broadcastPacket(PLAYER_POSITION_BROADCAST, &pp, sizeof(PlayerPosition));
}

void setPlayerPositions(Player* player, PlayerPosition* pos) {
	player->setHeadPosition(Vec3f(pos->headPosX, pos->headPosY, pos->headPosZ));
	player->setHeadRotation(Quaternion(pos->headRotX, pos->headRotY, pos->headRotZ, pos->headRotW));
	player->setDiskArmPosition(Vec3f(pos->rightPosX, pos->rightPosY, pos->rightPosZ));
	player->setDiskArmRotation(Quaternion(pos->rightRotX, pos->rightRotY, pos->rightRotZ, pos->rightRotW));
	player->setShieldArmPosition(Vec3f(pos->leftPosX, pos->leftPosY, pos->leftPosZ));
	player->setShieldArmRotation(Quaternion(pos->leftRotX, pos->leftRotY, pos->leftRotZ, pos->leftRotW));
}

void GameManager::handleGameTick() {
	if (playerBluePeer < 0) {
		aiBlue->update();
	}
	if (playerOrangePeer < 0) {
		aiOrange->update();
	}
	playerBlue->update();
	playerOrange->update();
	broadcastPlayerPositions(playerBlue, PLAYER_FACTION_BLUE, playerBluePeer);
	broadcastPlayerPositions(playerOrange, PLAYER_FACTION_ORANGE, playerOrangePeer);
}

void GameManager::handleConnect(unsigned short peerId) {
	PlayerInformation info;
	info.playerId = peerId;
	if (playerBluePeer < 0) {
		playerBluePeer = peerId;
		info.factionId = PLAYER_FACTION_BLUE;
	} else if (playerOrangePeer < 0) {
		playerOrangePeer = peerId;
		info.factionId = PLAYER_FACTION_ORANGE;
	} else {
		// new connection on full server
		return;
	}
	_server->sendPacket(peerId, PLAYER_IDENTIFICATION, &info, sizeof(PlayerInformation), true);
}

void GameManager::handleDisconnect(unsigned short peerId) {
	if (playerBluePeer == peerId) {
		playerBluePeer = -1;
		aiBlue->resetState();
	} else if (playerOrangePeer == peerId) {
		playerOrangePeer = -1;
		aiOrange->resetState();
	} else {
		// connection not known on this server
		return;
	}
}

void GameManager::handleCToSPacket(unsigned short peerId, CToSPacketType* header, void* data, int size) {
	switch (*header) {
	case START_GAME_REQUEST:
		std::cout << "Start request header: " << gameRunning << std::endl;
		if (!gameRunning) {
			playerBlue->setLifeCount(lifeCounterMaxLife);
			playerBlue->getShield()->refillCharges();
			playerOrange->setLifeCount(lifeCounterMaxLife);
			playerOrange->getShield()->refillCharges();
			aiBlue->resetState();
			aiOrange->resetState();
			std::cout << "Game starting, have fun :)" << std::endl;
			gameRunning = true;
			GameInformation gi;
			gi.isRunning = gameRunning;
			_server->broadcastPacket(GAME_STATE_BROADCAST, &gi, sizeof(GameInformation), true);
		} else {
			std::cout << "Game allready running, wait for it to finish..." << std::endl;
		}
		break;
	case PLAYER_POSITION_INFORMATION:
		if (playerBluePeer == peerId) {
			setPlayerPositions(playerBlue, reinterpret_cast<PlayerPosition*>(data));
		} else if (playerOrangePeer == peerId) {
			playerOrangePeer = -1;
		} else {
			// connection not known on this server
			return;
		}
		break;
	case PLAYER_THROW_INFORMATION:
		break;
	}
	/*std::cout << playerBlue->getHeadPosition() << " " << playerBlue->getHeadRotation() << std::endl;
	std::cout << playerBlue->getDiskArmPosition() << " " << playerBlue->getDiskArmRotation() << std::endl;
	std::cout << playerBlue->getShieldArmPosition() << " " << playerBlue->getShieldArmRotation() << std::endl;*/
}