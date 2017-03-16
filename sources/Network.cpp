#include "Network.h"

#include <iostream>

Server::Server(unsigned int hostAdress, short port) {
	_enetAddress.host = hostAdress;
	_enetAddress.port = port;

	_enetHost = enet_host_create (&_enetAddress, 2, 2, 0, 0);
	if (_enetHost == NULL)
	{
		std::cerr << "An error occurred while trying to create an ENet server host" << std::endl;
		exit (EXIT_FAILURE);
	}
	std::cout << "Server starting up" << std::endl;
	_packetHandler = nullptr;
}

Server::~Server() {
	std::cout << "Shutting down server." << std::endl;
	enet_host_destroy(_enetHost);
}

void Server::setPacketHandler(CToSPacketHandler* handler) {
	_packetHandler = handler;
}

CToSPacketHandler* Server::getPacketHandler() {
	return _packetHandler;
}

void Server::pollNetworkEvents() {
	ENetEvent event;
	CToSPacketType* header;
	std::string serializedData;
	while (enet_host_service (_enetHost, &event, 0) > 0) {
		switch (event.type) 	{
		case ENET_EVENT_TYPE_CONNECT:
			std::cout << "A new client connected from "
						<< event.peer->address.host << ":"
						<< event.peer->address.port << std::endl;
			/* Store any relevant client information here. */
			_peers[event.peer->incomingPeerID] = event.peer;
			if (_packetHandler != nullptr) {
				_packetHandler->handleConnect(event.peer->incomingPeerID);
			}
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			header = reinterpret_cast<CToSPacketType*>(event.packet->data);
			serializedData = std::string(reinterpret_cast<const char*>(event.packet->data + sizeof(CToSPacketType)), event.packet->dataLength - sizeof(CToSPacketType));
			if (_packetHandler != nullptr) {
				_packetHandler->handleCToSPacket(event.peer->incomingPeerID, header, serializedData);
			}
			enet_packet_destroy(event.packet);
			break;
       
		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout << "'" << event.peer->data << "' disconnected" << std::endl;
			/* Reset the peer's client information. */
			if (_packetHandler != nullptr) {
				_packetHandler->handleDisconnect(event.peer->incomingPeerID);
			}
			_peers.erase(event.peer->incomingPeerID);
			event.peer->data = NULL;
		}
	}
}
