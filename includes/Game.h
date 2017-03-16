
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
	Server *_server;
	Player *_playerBlue, *_playerOrange;
	AI *_aiBlue, *_aiOrange;
	int _playerBluePeer, _playerOrangePeer;
};

#endif