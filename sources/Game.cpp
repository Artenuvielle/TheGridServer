#include "Game.h"

GameManager::GameManager(Server* server) {
	_server = server;
	_playerBlue = new Player(PLAYER_FACTION_BLUE);
	_playerOrange = new Player(PLAYER_FACTION_ORANGE);
	_playerBlue->setEnemy(_playerOrange);
	_playerOrange->setEnemy(_playerBlue);
	_playerBlue->attach(this);
	_playerOrange->attach(this);
	_aiBlue = new AI(_playerBlue);
	_aiOrange = new AI(_playerOrange);
	_playerBluePeer = -1;
	_playerOrangePeer = -1;
}

GameManager::~GameManager() {
	delete _aiBlue, _aiOrange;
	delete _playerBlue;
	delete _playerOrange;
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
	_server->broadcastPacket(STOC_PACKET_TYPE_PLAYER_POSITION_BROADCAST, &pp, sizeof(PlayerPosition));
}

void setPlayerPositions(Player* player, PlayerPosition* pos) {
	player->setHeadPosition(Vec3f(pos->headPosX, pos->headPosY, pos->headPosZ));
	player->setHeadRotation(Quaternion(pos->headRotX, pos->headRotY, pos->headRotZ, pos->headRotW));
	player->setDiskArmPosition(Vec3f(pos->rightPosX, pos->rightPosY, pos->rightPosZ));
	player->setDiskArmRotation(Quaternion(pos->rightRotX, pos->rightRotY, pos->rightRotZ, pos->rightRotW));
	player->setShieldArmPosition(Vec3f(pos->leftPosX, pos->leftPosY, pos->leftPosZ));
	player->setShieldArmRotation(Quaternion(pos->leftRotX, pos->leftRotY, pos->leftRotZ, pos->leftRotW));
}

void throwPlayerDisk(Player* player, DiskThrowInformation* info) {
	player->getDisk()->forceThrow(Vec3f(info->diskPosX, info->diskPosY, info->diskPosZ), Vec3f(info->diskMomentumX, info->diskMomentumY, info->diskMomentumZ));
}

void GameManager::handleGameTick() {
	if (_playerBluePeer < 0) {
		_aiBlue->update();
	}
	if (_playerOrangePeer < 0) {
		_aiOrange->update();
	}
	_playerBlue->update();
	_playerOrange->update();
	broadcastPlayerPositions(_playerBlue, PLAYER_FACTION_BLUE, _playerBluePeer);
	if (_playerBlue->getDisk()->getState() == DISK_STATE_FREE_FLY || _playerBlue->getDisk()->getState() == DISK_STATE_RETURNING) {
		// send disk position
	}
	broadcastPlayerPositions(_playerOrange, PLAYER_FACTION_ORANGE, _playerOrangePeer);
	if (_playerOrange->getDisk()->getState() == DISK_STATE_FREE_FLY || _playerOrange->getDisk()->getState() == DISK_STATE_RETURNING) {
		// send disk position
	}
}

void GameManager::handleConnect(unsigned short peerId) {
	PlayerInformation info;
	info.playerId = peerId;
	if (_playerBluePeer < 0) {
		_playerBluePeer = peerId;
		info.factionId = PLAYER_FACTION_BLUE;
	} else if (_playerOrangePeer < 0) {
		_playerOrangePeer = peerId;
		info.factionId = PLAYER_FACTION_ORANGE;
	} else {
		// new connection on full server
		return;
	}
	_server->sendPacket(peerId, STOC_PACKET_TYPE_PLAYER_IDENTIFICATION, &info, sizeof(PlayerInformation), true);
}

void GameManager::handleDisconnect(unsigned short peerId) {
	if (_playerBluePeer == peerId) {
		_playerBluePeer = -1;
		_aiBlue->resetState();
	} else if (_playerOrangePeer == peerId) {
		_playerOrangePeer = -1;
		_aiOrange->resetState();
	} else {
		// connection not known on this server
		return;
	}
}

void GameManager::handleCToSPacket(unsigned short peerId, CToSPacketType* header, void* data, int size) {
	switch (*header) {
	case CTOS_PACKET_TYPE_START_GAME_REQUEST:
		std::cout << "Start request header: " << gameRunning << std::endl;
		if (!gameRunning) {
			_playerBlue->setLifeCount(lifeCounterMaxLife);
			_playerBlue->getShield()->refillCharges();
			_playerOrange->setLifeCount(lifeCounterMaxLife);
			_playerOrange->getShield()->refillCharges();
			_aiBlue->resetState();
			_aiOrange->resetState();
			std::cout << "Game starting, have fun :)" << std::endl;
			gameRunning = true;
			GameInformation gi;
			gi.isRunning = gameRunning;
			_server->broadcastPacket(STOC_PACKET_TYPE_GAME_STATE_BROADCAST, &gi, sizeof(GameInformation), true);
		} else {
			std::cout << "Game allready running, wait for it to finish..." << std::endl;
		}
		break;
	case CTOS_PACKET_TYPE_PLAYER_POSITION_INFORMATION:
		if (_playerBluePeer == peerId) {
			setPlayerPositions(_playerBlue, reinterpret_cast<PlayerPosition*>(data));
		} else if (_playerOrangePeer == peerId) {
			setPlayerPositions(_playerOrange, reinterpret_cast<PlayerPosition*>(data));
		} else {
			// connection not known on this server
			return;
		}
		break;
	case CTOS_PACKET_TYPE_PLAYER_THROW_INFORMATION:
		if (_playerBluePeer == peerId) {
			throwPlayerDisk(_playerBlue, reinterpret_cast<DiskThrowInformation*>(data));
		} else if (_playerOrangePeer == peerId) {
			throwPlayerDisk(_playerOrange, reinterpret_cast<DiskThrowInformation*>(data));
		} else {
			// connection not known on this server
			return;
		}
		break;
	}
	/*std::cout << playerBlue->getHeadPosition() << " " << playerBlue->getHeadRotation() << std::endl;
	std::cout << playerBlue->getDiskArmPosition() << " " << playerBlue->getDiskArmRotation() << std::endl;
	std::cout << playerBlue->getShieldArmPosition() << " " << playerBlue->getShieldArmRotation() << std::endl;*/
}

bool GameManager::observableUpdate(GameNotifications notification, Observable<GameNotifications>* src) {
	Player* srcPlayer = nullptr;
	int srcPeer;
	PlayerFaction srcFaction;
	if (_playerBlue == src) {
		std::cout << "Game event (blue): " << notification << std::endl;
		srcPlayer = _playerBlue;
		srcPeer = _playerBluePeer;
		srcFaction = PLAYER_FACTION_BLUE;
	} else {
		std::cout << "Game event (orange): " << notification << std::endl;
		srcPlayer = _playerOrange;
		srcPeer = _playerOrangePeer;
		srcFaction = PLAYER_FACTION_ORANGE;
	}
	if (srcPlayer != nullptr) {
		if (notification == GAME_NOTIFICATION_PLAYER_CHANGED_LIFE) {
			PlayerCounterInformation pci;
			pci.playerId = srcPeer;
			pci.factionId = srcFaction;
			pci.counter = srcPlayer->getLifeCount();
			_server->broadcastPacket(STOC_PACKET_TYPE_PLAYER_CHANGED_LIFE_BROADCAST, &pci, true);
		} else if (notification == GAME_NOTIFICATION_PLAYER_CHANGED_SHIELD_CHARGE) {
			PlayerCounterInformation pci;
			pci.playerId = srcPeer;
			pci.factionId = srcFaction;
			pci.counter = srcPlayer->getShield()->getCharges();
			_server->broadcastPacket(STOC_PACKET_TYPE_PLAYER_CHANGED_SHIELD_CHARGE_BROADCAST, &pci, true);
		} else if (notification == GAME_NOTIFICATION_DISK_STATE_CHANGED) {
		} else if (notification == GAME_NOTIFICATION_DISK_THROWN) {
			DiskThrowInformation dti;
			dti.playerId = srcPeer;
			dti.factionId = srcFaction;
			srcPlayer->getDisk()->getPosition().getSeparateValues(dti.diskPosX, dti.diskPosY, dti.diskPosZ);
			srcPlayer->getDisk()->getMomentum().getSeparateValues(dti.diskMomentumX, dti.diskMomentumY, dti.diskMomentumZ);
			_server->broadcastPacket(STOC_PACKET_TYPE_DISK_THROW_BROADCAST, &dti, true);
		}
	}
	return true;
}

void GameManager::observableRevoke(GameNotifications notification, Observable<GameNotifications>* src) {
	if (_playerBlue == src) {
		std::cout << "Game event revoked (blue): " << notification << std::endl;
	} else {
		std::cout << "Game event revoked (orange): " << notification << std::endl;
	}
}