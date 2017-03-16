#include <string>

struct PacketWraper {
	void* packetPointer;
	int size;
};

template<typename S> inline
PacketWraper combineHeaderAndPayload(SToCPacketType header, S* payload) {
	PacketWraper wraper;
	std::string serializedPayload;
	payload->SerializeToString(&serializedPayload);
	wraper.size = serializedPayload.size() + sizeof(SToCPacketType);
	void* packetData = malloc(wraper.size);
	memcpy(packetData, &header, sizeof(SToCPacketType));
	memcpy(reinterpret_cast<unsigned char *>(packetData) + sizeof(SToCPacketType), serializedPayload.c_str(), serializedPayload.size());
	wraper.packetPointer = packetData;
	return wraper;
}

template<typename S> inline
void Server::sendPacket(unsigned short peerId, SToCPacketType header, S* payload, bool reliable) {
	PacketWraper wraper = combineHeaderAndPayload(header, payload);
	ENetPacket* packet = enet_packet_create(wraper.packetPointer, wraper.size, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
	enet_peer_send(_peers[peerId], 0, packet);
	enet_host_flush(_enetHost);
	free(wraper.packetPointer);
}

template<typename S> inline
void Server::broadcastPacket(SToCPacketType header, S* payload, bool reliable) {
	PacketWraper wraper = combineHeaderAndPayload(header, payload);
	ENetPacket* packet = enet_packet_create(wraper.packetPointer, wraper.size, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
	enet_host_broadcast(_enetHost, 0, packet);
	enet_host_flush(_enetHost);
	free(wraper.packetPointer);
}
