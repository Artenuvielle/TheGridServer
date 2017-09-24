#include "AIStates/AIIdleState.h"

#include "Common.h"

AIIdleState::AIIdleState(Player* me) : AIStateHandler(me) {
	minTimeForAttack = 0;
}

AIState AIIdleState::update() {
	headPosition = (me->getFaction() == enemyFaction ? aiDefaultHeadPositionEnemy : aiDefaultHeadPositionUser) + Vec3f(cos(elapsedTime / 1000.f), sin(elapsedTime / 1000.f) * cos(elapsedTime / 1000.f)) * 10;
	Vec3f armDistance = me->getDiskArmPosition() - me->getShieldArmPosition();
	headRotation = Quaternion(Vec3f(0, 0, me->getFaction() == enemyFaction ? 1 : -1), me->getEnemy()->getHeadPosition() - me->getHeadPosition());
	Vec3f rotatedShoulderOffset(me->getFaction() == enemyFaction ? 25 : -25,0,0);
	me->getHeadRotation().multVec(rotatedShoulderOffset, rotatedShoulderOffset);
	
	diskArmPosition = me->getHeadPosition() + Vec3f(0,-60,0) - rotatedShoulderOffset;
	diskArmRotation = Quaternion(Vec3f(1,0,0), tg_math::degree2Rad(90)) * Quaternion(Vec3f(0,0,1), tg_math::degree2Rad(90));

	shieldArmPosition = me->getHeadPosition() + Vec3f(0,-60,0) + rotatedShoulderOffset;
	shieldArmRotation = Quaternion(Vec3f(1,0,0), tg_math::degree2Rad(90)) * Quaternion(Vec3f(0,0,1), tg_math::degree2Rad(-90));
	
	if (me->getDisk()->getState() == DISK_STATE_READY) {
		if (minTimeForAttack == 0) {
			minTimeForAttack = elapsedTime + (aiMinTimeUntilAttack + tg_math::random() * 3.f) * 1000;
		} else if(elapsedTime > minTimeForAttack) {
			return AI_STATE_ATTACK;
		}
	} else {
		if (me->getFaction() == enemyFaction) {
			if (me->getDisk()->getState() == DISK_STATE_RETURNING && me->getDisk()->getPosition().z() < WALL_Z_MID + 150) {
				return AI_STATE_CATCH;
			}
		} else {
			if (me->getDisk()->getState() == DISK_STATE_RETURNING && me->getDisk()->getPosition().z() > WALL_Z_MID - 150) {
				return AI_STATE_CATCH;
			}
		}
	}
	if (me->getFaction() == enemyFaction) {
		if (me->getEnemy()->getDisk()->getState() == DISK_STATE_FREE_FLY && me->getEnemy()->getDisk()->getPosition().z() < WALL_Z_MID) {
			return AI_STATE_DEFEND;
		}
	} else {
		if (me->getEnemy()->getDisk()->getState() == DISK_STATE_FREE_FLY && me->getEnemy()->getDisk()->getPosition().z() < WALL_Z_MID) {
			return AI_STATE_DEFEND;
		}
	}
	return AI_STATE_IDLE;
}