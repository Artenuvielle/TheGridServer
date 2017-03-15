
#ifndef _Network_H_
#define _Network_H_

#include "NetworkPackets.h"
#include <map>
#include <enet/enet.h>

class CToSPacketHandler {
public:
	virtual void handleConnect(unsigned short peerId) = 0;
	virtual void handleDisconnect(unsigned short peerId) = 0;
	virtual void handleCToSPacket(unsigned short peerId, CToSPacketType* header, void* data, int size) = 0; 
};

class Server {
public:
	Server(unsigned int hostAdress, short port);
	~Server();

	void setPacketHandler(CToSPacketHandler* handler);
	CToSPacketHandler* getPacketHandler();

	void pollNetworkEvents();
	void sendPacket(unsigned short peerId, SToCPacketType header, void* data, int size, bool reliable = false);
	void broadcastPacket(SToCPacketType header, void* data, int size, bool reliable = false);
private:
	CToSPacketHandler* _packetHandler;

	ENetAddress _enetAddress;
	ENetHost* _enetHost;
	std::map<enet_uint16, ENetPeer*> _peers;
};

#endif