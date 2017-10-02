#include "AIStates/AIDefendState.h"

#include "Common.h"
	
AIDefendState::AIDefendState(Player* me) : AIStateHandler(me) {
	calculateNewTargetPosition();
	startTime = elapsedTime;
}

Vec3f AIDefendState::calculateNewTargetPosition() {
	Vec3f p1 = me->getHeadPosition();
	Vec3f p2 = me->getEnemy()->getDisk()->getPosition();
	Vec3f targetDirection = p2 - p1;
	targetDirection.normalize();
	targetDirection *= aiDefendArmTorsoDistance;
	return targetDirection + p1;
}

AIState AIDefendState::update() {
	Vec3f rotatedShoulderOffset(25,0,0);
	me->getHeadRotation().multVec(rotatedShoulderOffset, rotatedShoulderOffset);
	
	if (elapsedTime - startTime > 200) {
		targetShieldArmPosition = calculateNewTargetPosition();
		startTime = elapsedTime;
	}
	shieldArmPosition = targetShieldArmPosition;
	Vec3f shieldArmDirection = me->getShieldArmPosition() - me->getTorsoPosition();
	shieldArmRotation = Quaternion(Vec3f(0,1,0), Vec3f(shieldArmDirection.x(), 0, shieldArmDirection.z()));// * Quaternion(Vec3f(1,0,0), osgDegree2Rad(-90));

	Vec3f shoulderPosition = me->getTorsoPosition() + rotatedShoulderOffset;
	Vec3f armDirection = me->getShieldArmPosition() - me->getTorsoPosition();
	armDirection.normalize();

	Vec3f ellbowDirection;
	me->getShieldArmRotation().multVec(Vec3f(0,0,-30), ellbowDirection);
	Vec3f ellbowPosition = me->getShieldArmPosition() + ellbowDirection;
	Vec3f shoulderDirection = me->getTorsoPosition() - ellbowPosition;
	shoulderDirection.normalize();
	shoulderDirection *= 30;
	Vec3f nextShoulderPosition = ellbowPosition + shoulderDirection;
	Vec3f shoulderOffset = nextShoulderPosition - shoulderPosition;
	
	headPosition = me->getHeadPosition() + shoulderOffset;
	headRotation = Quaternion(Vec3f(0, 0, 1), me->getEnemy()->getDisk()->getPosition() - me->getHeadPosition());
		
	diskArmPosition = me->getHeadPosition() + Vec3f(0,-60,0) - rotatedShoulderOffset;
	diskArmRotation = Quaternion(Vec3f(1,0,0), tg_math::degree2Rad(90)) * Quaternion(Vec3f(0,0,1), tg_math::degree2Rad(90));
	if (me->getDisk()->getState() == DISK_STATE_RETURNING && me->getDisk()->getPosition().z() < WALL_BACKWARD_MID) {
		return AI_STATE_CATCH;
	} else if (me->getEnemy()->getDisk()->getState() == DISK_STATE_FREE_FLY) {
		return AI_STATE_DEFEND;
	} else {
		return AI_STATE_IDLE;
	}
}