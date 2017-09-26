#include "Game.h"

PositionPacketType createPosition(Vec3f originPos, PlayerFaction faction, bool isDirection = false) {
	Vec3f pos = originPos;
	if (faction == enemyFaction) {
		if (isDirection) {
			Quaternion(Vec3f(0,1,0), tg_math::degree2Rad(180)).multVec(originPos, pos);
		} else {
			Quaternion(Vec3f(0,1,0), tg_math::degree2Rad(180)).multVec(originPos - Vec3f(0, 135, -405), pos);
			pos = pos + Vec3f(0, 135, -405);
		}
	}
	PositionPacketType ret;
    ret.set_x(pos.x());
    ret.set_y(pos.y());
    ret.set_z(pos.z());
    return ret;
}

OrientationPacketType createOrientation(Quaternion originRot, PlayerFaction faction) {
	Quaternion rot = originRot;
	if (faction == enemyFaction) {
		//std::cout << "sending rotated quat" << std::endl;
		//rot = originRot * Quaternion(Vec3f(0,1,0), tg_math::degree2Rad(180));
	}
    OrientationPacketType ret;
    ret.set_x(rot.x());
    ret.set_y(rot.y());
    ret.set_z(rot.z());
    ret.set_w(rot.w());
    return ret;
}

Vec3f createVector(PositionPacketType pos, PlayerFaction faction, bool isDirection = false) {
	if (faction == enemyFaction) {
		Vec3f rotatedPos;
		if (isDirection) {
			Quaternion(Vec3f(0,1,0), tg_math::degree2Rad(180)).multVec(Vec3f(pos.x(), pos.y(), pos.z()), rotatedPos);
			return rotatedPos;
		} else {
			Quaternion(Vec3f(0,1,0), tg_math::degree2Rad(180)).multVec(Vec3f(pos.x(), pos.y() - 135, pos.z() + 405), rotatedPos);
			return rotatedPos + Vec3f(0, 135, -405);
		}
	}
	return Vec3f(pos.x(), pos.y(), pos.z());
}

Quaternion createQuaternion(OrientationPacketType rot, PlayerFaction faction) {
	if (faction == enemyFaction) {
		//std::cout << "receiving rotated quat" << std::endl;
		//return Quaternion(rot.x(), rot.y(), rot.z(), rot.w()) * Quaternion(Vec3f(0,1,0), tg_math::degree2Rad(180));
	}
	return Quaternion(rot.x(), rot.y(), rot.z(), rot.w());
}

template<typename S> inline
S* deserialize(std::string serializedData) {
	S* ret = new S();
	ret->ParseFromString(serializedData);
	return ret;
}

void setPlayerPositions(Player* player, PlayerPosition* pos) {
	/*if (player->getFaction() == userFaction) {
		Quaternion vec = createQuaternion(pos->main_hand_rot(), player->getFaction());
		std::cout << "x: " << vec.x() << " y: " << vec.y() << " z: " << vec.z() << " w: " << vec.w() << std::endl;
	}*/
	player->setHeadPosition(createVector(pos->head_pos(), player->getFaction()));
	player->setHeadRotation(createQuaternion(pos->head_rot(), player->getFaction()));
	player->setDiskArmPosition(createVector(pos->main_hand_pos(), player->getFaction()));
	player->setDiskArmRotation(createQuaternion(pos->main_hand_rot(), player->getFaction()));
	player->setShieldArmPosition(createVector(pos->off_hand_pos(), player->getFaction()));
	player->setShieldArmRotation(createQuaternion(pos->off_hand_rot(), player->getFaction()));
}

void throwPlayerDisk(Player* player, DiskThrowInformation* info) {
	player->getDisk()->forceThrow(createVector(info->disk_pos(), player->getFaction()), createVector(info->disk_momentum(), player->getFaction(), true));
}

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
	if (_playerBluePeer >= 0) {
		sendPlayerPositionsToPeer(player, faction, playerId, _playerBluePeer);
	}
	if (_playerOrangePeer >= 0) {
		sendPlayerPositionsToPeer(player, faction, playerId, _playerOrangePeer);
	}
}

void GameManager::sendPlayerPositionsToPeer(Player* player, PlayerFaction faction, int playerId, int peerId) {
	PlayerFaction receivingFaction = peerId == _playerBluePeer ? PLAYER_FACTION_BLUE : PLAYER_FACTION_ORANGE;
	PlayerPosition* pp = new PlayerPosition();
	pp->set_player_id(playerId);
	pp->set_faction_id(faction);
	PositionPacketType head_pos = createPosition(player->getHeadPosition(), receivingFaction);
	PositionPacketType main_hand_pos = createPosition(player->getDiskArmPosition(), receivingFaction);
	PositionPacketType off_hand_pos = createPosition(player->getShieldArmPosition(), receivingFaction);
	OrientationPacketType head_rot = createOrientation(player->getHeadRotation(), receivingFaction);
	OrientationPacketType main_hand_rot = createOrientation(player->getDiskArmRotation(), receivingFaction);
	OrientationPacketType off_hand_rot = createOrientation(player->getShieldArmRotation(), receivingFaction);
	pp->set_allocated_head_pos(&head_pos);
	pp->set_allocated_head_rot(&head_rot);
	pp->set_allocated_main_hand_pos(&main_hand_pos);
	pp->set_allocated_main_hand_rot(&main_hand_rot);
	pp->set_allocated_off_hand_pos(&off_hand_pos);
	pp->set_allocated_off_hand_rot(&off_hand_rot);
	_server->sendPacket(peerId, STOC_PACKET_TYPE_PLAYER_POSITION_BROADCAST, pp);
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
		broadcastDiscPosition(_playerBlue, PLAYER_FACTION_BLUE, _playerBluePeer);
	}
	broadcastPlayerPositions(_playerOrange, PLAYER_FACTION_ORANGE, _playerOrangePeer);
	if (_playerOrange->getDisk()->getState() == DISK_STATE_FREE_FLY || _playerOrange->getDisk()->getState() == DISK_STATE_RETURNING) {
		broadcastDiscPosition(_playerOrange, PLAYER_FACTION_ORANGE, _playerOrangePeer);
	}
}

void GameManager::handleConnect(unsigned short peerId) {
	PlayerInformation* info = new PlayerInformation();
	info->set_player_id(peerId);
	if (_playerBluePeer < 0) {
		_playerBluePeer = peerId;
		info->set_faction_id(PLAYER_FACTION_BLUE);
	} else if (_playerOrangePeer < 0) {
		_playerOrangePeer = peerId;
		info->set_faction_id(PLAYER_FACTION_ORANGE);
	} else {
		// new connection on full server
		return;
	}
	_server->sendPacket<PlayerInformation>(peerId, STOC_PACKET_TYPE_PLAYER_IDENTIFICATION, info, true);
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

void GameManager::handleCToSPacket(unsigned short peerId, CToSPacketType* header, std::string data) {
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
			GameInformation* gi = new GameInformation();
			gi->set_is_running(gameRunning);
			_server->broadcastPacket<GameInformation>(STOC_PACKET_TYPE_GAME_STATE_BROADCAST, gi, true);
		} else {
			std::cout << "Game allready running, wait for it to finish..." << std::endl;
		}
		break;
	case CTOS_PACKET_TYPE_PLAYER_POSITION_INFORMATION:
		if (_playerBluePeer == peerId) {
			setPlayerPositions(_playerBlue, deserialize<PlayerPosition>(data));
		} else if (_playerOrangePeer == peerId) {
			setPlayerPositions(_playerOrange, deserialize<PlayerPosition>(data));
		} else {
			// connection not known on this server
			return;
		}
		break;
	case CTOS_PACKET_TYPE_PLAYER_THROW_INFORMATION:
		if (_playerBluePeer == peerId) {
			throwPlayerDisk(_playerBlue, deserialize<DiskThrowInformation>(data));
		} else if (_playerOrangePeer == peerId) {
			throwPlayerDisk(_playerOrange, deserialize<DiskThrowInformation>(data));
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
			PlayerCounterInformation* pci = new PlayerCounterInformation();
			pci->set_player_id(srcPeer);
			pci->set_faction_id(srcFaction);
			pci->set_counter(srcPlayer->getLifeCount());
			_server->broadcastPacket<PlayerCounterInformation>(STOC_PACKET_TYPE_PLAYER_CHANGED_LIFE_BROADCAST, pci, true);
		} else if (notification == GAME_NOTIFICATION_PLAYER_CHANGED_SHIELD_CHARGE) {
			PlayerCounterInformation* pci = new PlayerCounterInformation();
			pci->set_player_id(srcPeer);
			pci->set_faction_id(srcFaction);
			pci->set_counter(srcPlayer->getShield()->getCharges());
			_server->broadcastPacket<PlayerCounterInformation>(STOC_PACKET_TYPE_PLAYER_CHANGED_SHIELD_CHARGE_BROADCAST, pci, true);
		} else if (notification == GAME_NOTIFICATION_DISK_STATE_CHANGED) {
			DiskStatusInformation* dsi = new DiskStatusInformation();
			dsi->set_player_id(srcPeer);
			dsi->set_faction_id(srcFaction);
			dsi->set_disk_status_id(srcPlayer->getDisk()->getState());
			_server->broadcastPacket<DiskStatusInformation>(STOC_PACKET_TYPE_DISK_STATUS_BROADCAST, dsi, true);
		} else if (notification == GAME_NOTIFICATION_DISK_THROWN) {
			broadcastDiscThrowInformation(srcPlayer, srcFaction, srcPeer);
		}
	}
	return true;
}

void GameManager::broadcastDiscThrowInformation(Player* player, PlayerFaction faction, int playerId) {
	if (_playerBluePeer >= 0) {
		sendDiscThrowInformationToPeer(player, faction, playerId, _playerBluePeer);
	}
	if (_playerOrangePeer >= 0) {
		sendDiscThrowInformationToPeer(player, faction, playerId, _playerOrangePeer);
	}
}

void GameManager::sendDiscThrowInformationToPeer(Player* player, PlayerFaction faction, int playerId, int peerId) {
	PlayerFaction receivingFaction = peerId == _playerBluePeer ? PLAYER_FACTION_BLUE : PLAYER_FACTION_ORANGE;
	DiskThrowInformation* dti = new DiskThrowInformation();
	dti->set_player_id(playerId);
	dti->set_faction_id(faction);
	PositionPacketType diskPosition = createPosition(player->getDisk()->getPosition(), receivingFaction);
	PositionPacketType diskMomentum = createPosition(player->getDisk()->getMomentum(), receivingFaction);
	dti->set_allocated_disk_pos(&diskPosition);
	dti->set_allocated_disk_momentum(&diskMomentum);
	_server->sendPacket<DiskThrowInformation>(peerId, STOC_PACKET_TYPE_DISK_THROW_BROADCAST, dti, true);
}

void GameManager::broadcastDiscPosition(Player* player, PlayerFaction faction, int playerId) {
	if (_playerBluePeer >= 0) {
		sendDiscPosition(player, faction, playerId, _playerBluePeer);
	}
	if (_playerOrangePeer >= 0) {
		sendDiscPosition(player, faction, playerId, _playerOrangePeer);
	}
}

void GameManager::sendDiscPosition(Player* player, PlayerFaction faction, int playerId, int peerId) {
	PlayerFaction receivingFaction = peerId == _playerBluePeer ? PLAYER_FACTION_BLUE : PLAYER_FACTION_ORANGE;
	DiskPosition* dp = new DiskPosition();
	dp->set_player_id(playerId);
	dp->set_faction_id(faction);
	PositionPacketType diskPosition = createPosition(player->getDisk()->getPosition(), receivingFaction);
	dp->set_allocated_disk_pos(&diskPosition);
	_server->sendPacket<DiskPosition>(peerId, STOC_PACKET_TYPE_DISK_POSITION_BROADCAST, dp);
}

void GameManager::observableRevoke(GameNotifications notification, Observable<GameNotifications>* src) {
	if (_playerBlue == src) {
		std::cout << "Game event revoked (blue): " << notification << std::endl;
	} else {
		std::cout << "Game event revoked (orange): " << notification << std::endl;
	}
}