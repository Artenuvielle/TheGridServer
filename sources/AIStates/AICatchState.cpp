#include "AIStates/AICatchState.h"

#include "Common.h"
	
AICatchState::AICatchState(Player* me) : AIStateHandler(me) {
	calculateNewTargetPosition();
	startTime = elapsedTime;
}

Vec3f AICatchState::calculateNewTargetPosition() {
	Vec3f p1 = me->getDiskArmPosition();
	Vec3f p2 = me->getDisk()->getPosition();
	Vec3f targetDirection = p2 - p1;
	targetDirection.normalize();
	targetDirection *= aiCatchArmTorsoDistance;
	return Vec3f(p2.x(), p2.y(), tg_math::max(p2.z(), me->getHeadPosition().z() + 70));//targetDirection + p1;
}

AIState AICatchState::update() {
	Vec3f rotatedShoulderOffset(25,0,0);
	me->getHeadRotation().multVec(rotatedShoulderOffset, rotatedShoulderOffset);
	
	if (elapsedTime - startTime > 200) {
		targetDiskArmPosition = calculateNewTargetPosition();
		startTime = elapsedTime;
	}
	diskArmPosition = targetDiskArmPosition;
	Vec3f diskArmDirection = me->getDiskArmPosition() - me->getTorsoPosition();
	diskArmRotation = Quaternion(Vec3f(0,1,0), Vec3f(diskArmDirection.x(), 0, diskArmDirection.z()));// * Quaternion(Vec3f(1,0,0), osgDegree2Rad(-90));

	Vec3f shoulderPosition = me->getTorsoPosition() - rotatedShoulderOffset;
	Vec3f armDirection = me->getDiskArmPosition() - me->getTorsoPosition();
	armDirection.normalize();

	Vec3f ellbowDirection;
	me->getDiskArmRotation().multVec(Vec3f(0,0,-30), ellbowDirection);
	Vec3f ellbowPosition = me->getDiskArmPosition() + ellbowDirection;
	Vec3f shoulderDirection = me->getTorsoPosition() - ellbowPosition;
	shoulderDirection.normalize();
	shoulderDirection *= 30;
	Vec3f nextShoulderPosition = ellbowPosition + shoulderDirection;
	Vec3f shoulderOffset = nextShoulderPosition - shoulderPosition;
	
	headPosition = me->getHeadPosition() + shoulderOffset;
	headRotation = Quaternion(Vec3f(0, 0, 1), me->getDisk()->getPosition() - me->getHeadPosition());
		
	shieldArmPosition = me->getHeadPosition() + Vec3f(0,-60,0) + rotatedShoulderOffset;
	shieldArmRotation = Quaternion(Vec3f(1,0,0), tg_math::degree2Rad(90)) * Quaternion(Vec3f(0,0,1), tg_math::degree2Rad(-90));
	if (me->getDisk()->getState() == DISK_STATE_RETURNING) {
		return AI_STATE_CATCH;
	} else {
		return AI_STATE_IDLE;
	}
}