
#ifndef _Network_H_
#define _Network_H_

#include "NetworkPackets.h"
#include <map>
#include <enet/enet.h>

class CToSPacketHandler {
public:
	virtual void handleConnect(unsigned short peerId) = 0;
	virtual void handleDisconnect(unsigned short peerId) = 0;
	virtual void handleCToSPacket(unsigned short peerId, ProtobufMessagePacket* packet) = 0;
};

class Server {
public:
	Server(unsigned int hostAdress, short port);
	~Server();

	void setPacketHandler(CToSPacketHandler* handler);
	CToSPacketHandler* getPacketHandler();

	void pollNetworkEvents();
	void sendPacket(unsigned short peerId, ProtobufMessagePacket* payload, bool reliable = false);
	void broadcastPacket(ProtobufMessagePacket* payload, bool reliable = false);
private:
	CToSPacketHandler* _packetHandler;

	ENetAddress _enetAddress;
	ENetHost* _enetHost;
	std::map<enet_uint16, ENetPeer*> _peers;
};
#endif
