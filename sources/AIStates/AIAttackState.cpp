#include "AIStates/AIAttackState.h"

#include "Common.h"

Real32 quadRand(Real32 scale) {
	Real32 r = tg_math::random();
	return (r*scale*r*scale);
}

AIAttackState::AIAttackState(Player* me) : AIStateHandler(me) {
	startPosition = diskArmPosition;
	startDirection = Vec3f(0, 0, 0);
	
	Vec3f rotatedShoulderOffset(25,0,0);
	headRotation.multVec(rotatedShoulderOffset, rotatedShoulderOffset);

	Vec3f randomDrawStartDirection(tg_math::random() - 0.3f, (tg_math::random() - 0.7f) * 0.7f, (tg_math::random() - 0.8f) * 0.2f);
	randomDrawStartDirection.normalize();
	startDrawingPosition = me->getTorsoPosition() - rotatedShoulderOffset + randomDrawStartDirection * tg_math::pow(tg_math::random(), 2) * 40;
	startDrawingPosition = getPositionForAIInBounds(startDrawingPosition);
	
	startDrawingDirection = Vec3f(0.f, 0.f, -1.f);
	
	Vec3f randomDrawEndDirection((tg_math::random() - 0.5f) * 0.5f, (tg_math::random() - 0.5f) * 0.5f, tg_math::random());
	randomDrawEndDirection.normalize();
	endDrawingPosition = startDrawingPosition + randomDrawEndDirection * (tg_math::pow(tg_math::random(), 2) * 20 + 50);
	endDrawingPosition = getPositionForAIInBounds(endDrawingPosition);
	
	endDrawingDirection = endDrawingPosition - startDrawingPosition;
	//endDrawingDirection.normalize();
	
	distanceToDrawStart = splineLengthApproximation(4, startPosition, startDrawingPosition, startDirection, startDrawingDirection);
	distanceToDrawEnd = splineLengthApproximation(4, startDrawingPosition, endDrawingPosition, startDrawingDirection, endDrawingDirection);
	startTime = elapsedTime;
	isDrawing = false;
}

AIState AIAttackState::update() {
	AIState nextState = AI_STATE_ATTACK;
	Real32 splinePercent = (elapsedTime - startTime) / 1000;
	if (isDrawing) {
		if (splinePercent >= 1) {
			nextState = AI_STATE_IDLE;
			diskArmPosition = endDrawingPosition;
			me->getDisk()->endDraw(me->getDiskArmPosition());
		} else {
			diskArmPosition = splineInterpolation(splinePercent, startDrawingPosition, endDrawingPosition, startDrawingDirection, endDrawingDirection);
		}
	} else {
		if (splinePercent >= 1) {
			startTime = elapsedTime;
			isDrawing = true;
			diskArmPosition = startDrawingPosition;
			me->getDisk()->startDraw(me->getDiskArmPosition());
		} else {
			diskArmPosition = splineInterpolation(splinePercent, startPosition, startDrawingPosition, startDirection, startDrawingDirection);
		}
	}
	Vec3f rotatedShoulderOffset(25,0,0);
	me->getHeadRotation().multVec(rotatedShoulderOffset, rotatedShoulderOffset);
	
	Vec3f shoulderPosition = me->getTorsoPosition() - rotatedShoulderOffset;
	Vec3f armDirection = me->getDiskArmPosition() - me->getTorsoPosition();
	armDirection.normalize();
	//std::cout << armDirection << "\n";
	if (isDrawing) {
		diskArmRotation = Quaternion(Vec3f(0,1,0), Vec3f(0,0,1).enclosedAngle(Vec3f(armDirection.x(), 0, armDirection.z())));
	} else {
		diskArmRotation = Quaternion(Vec3f(0,1,0), Vec3f(0,0,1).enclosedAngle(Vec3f(armDirection.x(), 0, armDirection.z()))) * Quaternion(Vec3f(1,0,0), tg_math::degree2Rad(90 * (1 - splinePercent)));
	}

	Vec3f ellbowDirection;
	me->getDiskArmRotation().multVec(Vec3f(0,0,-30), ellbowDirection);
	Vec3f ellbowPosition = me->getDiskArmPosition() + ellbowDirection;
	Vec3f shoulderDirection = me->getTorsoPosition() - ellbowPosition;
	shoulderDirection.normalize();
	shoulderDirection = Real32(30) * shoulderDirection;
	Vec3f nextShoulderPosition = ellbowPosition + shoulderDirection;
	Vec3f shoulderOffset = nextShoulderPosition - shoulderPosition;
	
	headPosition = me->getHeadPosition() + shoulderOffset;
	Vec3f armDistance = me->getDiskArmPosition() - me->getShieldArmPosition();
	headRotation = Quaternion(Vec3f(-1, 0, 0), Vec3f(armDistance.x(), 0, armDistance.z()));

	shieldArmPosition = me->getHeadPosition() + Vec3f(0,-60,0) + rotatedShoulderOffset;
	shieldArmRotation = Quaternion(Vec3f(1,0,0), tg_math::degree2Rad(90)) * Quaternion(Vec3f(0,0,1), tg_math::degree2Rad(-90));

	return nextState;
}