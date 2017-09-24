
#ifndef _Game_H_
#define _Game_H_

#include "Observer.h"
#include "Network.h"
#include "Common.h"
#include "Player.h"
#include "ArtificialIntelligence.h"

class GameManager : public CToSPacketHandler, public Observer<GameNotifications> {
public:
	GameManager(Server* server);
	~GameManager();

	void handleGameTick();

	void handleConnect(unsigned short peerId) override;
	void handleDisconnect(unsigned short peerId) override;
	void handleCToSPacket(unsigned short peerId, CToSPacketType* header, std::string data) override;

	bool observableUpdate(GameNotifications notification, Observable<GameNotifications>* src) override;
	void observableRevoke(GameNotifications notification, Observable<GameNotifications>* src) override;

private:
	void broadcastPlayerPositions(Player* player, PlayerFaction faction, int playerId);
	void sendPlayerPositionsToPeer(Player* player, PlayerFaction faction, int playerId, int peerId);
	void broadcastDiscThrowInformation(Player* player, PlayerFaction faction, int playerId);
	void sendDiscThrowInformationToPeer(Player* player, PlayerFaction faction, int playerId, int peerId);
	void broadcastDiscPosition(Player* player, PlayerFaction faction, int playerId);
	void sendDiscPosition(Player* player, PlayerFaction faction, int playerId, int peerId);
	Server *_server;
	Player *_playerBlue, *_playerOrange;
	AI *_aiBlue, *_aiOrange;
	int _playerBluePeer, _playerOrangePeer;
};

#endif