
#ifndef _AICatchState_H_
#define _AICatchState_H_

#include "ArtificialIntelligence.h"

class AICatchState : public AIStateHandler {
public:
	AIState update();
	AICatchState(Player* me);
private:
	Real32 startTime;
	Vec3f calculateNewTargetPosition();
	Vec3f targetDiskArmPosition;
};

#endif