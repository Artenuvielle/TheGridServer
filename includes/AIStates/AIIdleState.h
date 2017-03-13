
#ifndef _AIIdleState_H_
#define _AIIdleState_H_

#include "ArtificialIntelligence.h"

class AIIdleState : public AIStateHandler {
public:
	AIState update();
	AIIdleState(Player* me);
private:
	Real32 minTimeForAttack;
};

#endif