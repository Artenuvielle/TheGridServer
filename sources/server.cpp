#include <iostream>
#include <enet/enet.h>

//#define OUTPUT_TICKS_EVERY_MILLISECONDS 10000

#include "Network.h"
#include "Game.h"
#include "Player.h"

Server* gameServer;
GameManager* gameManager;

int maxServerTicksPerSecond = 50;

void onClose() {
	delete gameServer;
	enet_deinitialize();
	delete gameManager;
}

void sleepUntilTickTimeIsOver() {
	int timeToSleep = 1000 / maxServerTicksPerSecond;
	Sleep(timeToSleep);
	elapsedTime += timeToSleep;
}

int main (int argc, char ** argv) {
    if (enet_initialize () != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        throw 0;
    }
    atexit(onClose);
	elapsedTime = 0;

	Player a(userFaction);
	
	gameServer = new Server(ENET_HOST_ANY, 13244);
	gameManager = new GameManager(gameServer);
	gameServer->setPacketHandler(gameManager);
	
#ifdef OUTPUT_TICKS_EVERY_MILLISECONDS
	int tickCount = 0;
	Real32 lastTickOutput = elapsedTime;
#endif
	while (true)
	{
#ifdef OUTPUT_TICKS_EVERY_MILLISECONDS
		tickCount++;
#endif
		gameServer->pollNetworkEvents();
		gameManager->handleGameTick();
		sleepUntilTickTimeIsOver();
#ifdef OUTPUT_TICKS_EVERY_MILLISECONDS
		if (elapsedTime - lastTickOutput >= OUTPUT_TICKS_EVERY_MILLISECONDS) {
			std::cout << "server running " << tickCount * 1000 / (elapsedTime - lastTickOutput) << " ticks/s" << std::endl;
			tickCount = 0;
			lastTickOutput = elapsedTime;
		}
#endif
	}
}