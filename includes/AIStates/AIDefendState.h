
#ifndef _AIDefendState_H_
#define _AIDefendState_H_

#include "ArtificialIntelligence.h"

class AIDefendState : public AIStateHandler {
public:
	AIState update();
	AIDefendState(Player* me);
private:
	Real32 startTime;
	Vec3f calculateNewTargetPosition();
	Vec3f targetShieldArmPosition;
};

#endif