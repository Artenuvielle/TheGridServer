#include "Disk.h"

#include <iostream>

#include "Common.h"

Quaternion interpolateVector(Vec3f vec1, Vec3f vec2, Real32 factor);

Disk::Disk(PlayerFaction type, DiskEventHandler* handler) : handler(handler) {
	diskType = type;
	
	transform.setTranslation(Vec3f(0,135,-540));
	transform.setScale(Vec3f(diskRadius, diskHeight * 10 / 2, diskRadius));

	state = DISK_STATE_READY;

	rotationAroundAxis = 0;
}

Disk::~Disk(){}

bool Disk::setPosition(Vec3f newPosition) {
	if(state == DISK_STATE_READY || state == DISK_STATE_DRAWN) {
		if (state == DISK_STATE_DRAWN) {
			momentum = 0.9f * momentum + (newPosition - lastPositionWhileDrawn);
			lastPositionWhileDrawn = newPosition;
		}
		transform.setTranslation(newPosition);
		return true;
	}
	return false;
}

Vec3f Disk::getPosition() {
	return transform.getTranslation();
}

void Disk::setTargetOwnerPosition(Vec3f newPosition) {
	targetOwnerPosition = newPosition;
}

Vec3f Disk::getTargetOwnerPosition() {
	return targetOwnerPosition;
}

void Disk::setTargetEnemyPosition(Vec3f newPosition) {
	targetEnemyPosition = newPosition;
}

Vec3f Disk::getTargetEnemyPosition() {
	return targetEnemyPosition;
}

bool Disk::setRotation(Quaternion newRotation) {
	if(state == DISK_STATE_READY || state == DISK_STATE_DRAWN) {
		Vec3f newForward;
		newRotation.multVec(Vec3f(0,0,1), newForward);
		if (state == DISK_STATE_DRAWN) {
			rotationAroundAxis += tg_math::rad2Degree(newForward.enclosedAngle(lastFowardVectorWhileDrawn)) / (elapsedTime - lastRotationUpdateTime) * 1000;
		}
		lastFowardVectorWhileDrawn = newForward;
		transform.setRotation(newRotation);
		return true;
	}
	lastRotationUpdateTime = elapsedTime;
	return false;
}

void Disk::setEnemyShield(Shield* othersShield) {
	enemyShield = othersShield;
}

Quaternion Disk::getRotation() {
	return transform.getRotation();
}

DiskState Disk::getState() {
	return state;
}

Vec3f Disk::getMomentum() {
	return momentum;
}

bool Disk::startDraw(Vec3f position) {
	if(gameRunning && state == DISK_STATE_READY) {
		lastPositionWhileDrawn = position;
		momentum = Vec3f(0,0,0);
		state = DISK_STATE_DRAWN;
		std::cout << "started drawing a disk" << '\n';
		return true;
	}
	return false;
}

bool Disk::endDraw(Vec3f position) {
	if(state == DISK_STATE_DRAWN) {
		state = DISK_STATE_FREE_FLY;
		momentum.normalize();
		targetAngle = tg_math::rad2Degree(Vec3f(0,1,0).enclosedAngle(currentAxis));
		lastCollisionAngle = targetAngle;
		currentAngle = targetAngle;
		axialRotationPerMillisecond = 0;
		notify(GAME_NOTIFICATION_DISK_THROWN);
		std::cout << "finished drawing a disk... LET IF FLYYYYYY" << '\n';
		
		return true;
	}
	return false;
}

bool Disk::forceReturn() {
	if (state == DISK_STATE_FREE_FLY) {
		state = DISK_STATE_RETURNING;
		return true;
	}
	return false;
}

bool Disk::forceThrow(Vec3f position, Vec3f mom) {
	if (startDraw(position)) {
		momentum = mom;
		if (endDraw(position)) {
			return true;
		}
	}
	return false;
}

void Disk::catchDisk() {
	rotationAroundAxis = 0;
	state = DISK_STATE_READY;
}

Vec3f Disk::calculateMovement(Real32 deltaTime) {
	return deltaTime * diskSpeed * momentum;
}

void Disk::update() {
	transform.getRotation().multVec(Vec3f(0,1,0), currentAxis);
	if (state == DISK_STATE_FREE_FLY || state == DISK_STATE_RETURNING) {
		Vec3f vectorToTarget;
		Real32 diskMomentumAttractionFactor;
		Vec3f forward;
		if (state == DISK_STATE_FREE_FLY) {
			if ((transform.getTranslation().z() < targetEnemyPosition.z() && diskType == userFaction) ||
				(transform.getTranslation().z() > targetEnemyPosition.z() && diskType == enemyFaction)) {
				vectorToTarget = momentum;
			} else {
				vectorToTarget = targetEnemyPosition - transform.getTranslation();
			}
			diskMomentumAttractionFactor = diskEnemyMomentumAttractionFactor;
			forward = Vec3f(0,0,userFaction == diskType ? -1 : 1);
		} else {
			if ((transform.getTranslation().z() > targetOwnerPosition.z() && diskType == userFaction) ||
				(transform.getTranslation().z() < targetOwnerPosition.z() && diskType == enemyFaction)) {
				vectorToTarget = momentum;
			} else {
				vectorToTarget = targetOwnerPosition - transform.getTranslation();
			}
			diskMomentumAttractionFactor = diskOwnerMomentumAttractionFactor;
			forward = Vec3f(0,0,userFaction == diskType ? 1 : -1);
		}

		Real32 targetProximity = 1 - (vectorToTarget.length() / WALL_Z_DIFF);

		Quaternion axisRotation;
		if (elapsedTime - lastCollisionTime < diskRotationTimeAfterCollision * 1000) {
			Real32 angleDiff = targetAngle - lastCollisionAngle;
			if (angleDiff < -180) angleDiff += 360;
			if (angleDiff > 180) angleDiff -= 360;
			axialRotationPerMillisecond = angleDiff / (diskRotationTimeAfterCollision * 1000);
			Real32 newAngle = lastCollisionAngle + axialRotationPerMillisecond * (elapsedTime - lastCollisionTime);
			axisRotation = Quaternion(forward, tg_math::degree2Rad(newAngle));
			currentAngle = newAngle;
		} else {
			if (abs(axialRotationPerMillisecond) > diskMinimalAxialRotationAfterCollision) {
				axialRotationPerMillisecond *= pow(0.995f, (elapsedTime - lastCollisionTime));
				if (abs(axialRotationPerMillisecond) < diskMinimalAxialRotationAfterCollision * 1.1) {
					axialRotationPerMillisecond = tg_math::sgn(axialRotationPerMillisecond) * diskMinimalAxialRotationAfterCollision;
				}
			}
			targetAngle += axialRotationPerMillisecond * (elapsedTime - lastCollisionTime);
			if (targetAngle < -180) targetAngle += 360;
			if (targetAngle > 180) targetAngle -= 360;
			axisRotation = Quaternion(forward, tg_math::degree2Rad(targetAngle));
			currentAngle = targetAngle;
		}
		Quaternion rotation = interpolateVector(momentum, vectorToTarget, (elapsedTime - lastPositionUpdateTime) / 1000 * diskMomentumAttractionFactor * targetProximity * targetProximity);
		rotation.multVec(momentum, momentum);
		Quaternion aroundAxisRotation(Vec3f(0,-1,0), tg_math::degree2Rad((elapsedTime - lastPositionUpdateTime) * rotationAroundAxis / 1000 * diskAxisRotationFactor));
		transform.setRotation(Quaternion(forward, momentum) * axisRotation * aroundAxisRotation);

		//detecting wall collisions and moving
		moveDiskAtLeastUntilWallCollision(elapsedTime - lastPositionUpdateTime);
		// detecting enemy collisons
		if (state == DISK_STATE_FREE_FLY) {
			if (enemyShield->hasCharges() && collidesWithEnemyShield()) {
				std::cout << "Disk was defended with shield" << '\n';
				enemyShield->reduceCharges();
				state = DISK_STATE_RETURNING;
				// mirror momentum on shield surface
				Vec3f shieldNormal;
				enemyShield->getRotation().multVec(Vec3f(0, 1, 0), shieldNormal);
				momentum -= shieldNormal * 2 * (momentum.dot(shieldNormal));
			}
		}
	}
	lastPositionUpdateTime = elapsedTime;
}

void Disk::moveDiskAtLeastUntilWallCollision(Real32 deltaTime) {
	Real32 x,y,z;
	Real32 stepLengthPercentage = 1.f;
	Vec3f nextMomentum(momentum);
	Real32 nextTargetAngle;
	bool collided = false;
	transform.getTranslation().getSeparateValues(x,y,z);
	//transform.getRotation().multVec(Vec3f(0,1,0), currentAxis);
	Vec3f nearestXOffset = currentAxis.cross(Vec3f(1,0,0)).cross(currentAxis);
	Vec3f nearestYOffset = currentAxis.cross(Vec3f(0,1,0)).cross(currentAxis);
	Vec3f nearestZOffset = currentAxis.cross(Vec3f(0,0,1)).cross(currentAxis);
	nearestXOffset *= diskRadius / nearestXOffset.length();
	nearestYOffset *= diskRadius / nearestYOffset.length();
	nearestZOffset *= diskRadius / nearestZOffset.length();
	Vec3f moveVector = calculateMovement(deltaTime);
	
	if ((transform.getTranslation().x() + nearestXOffset.x() + moveVector.x() * stepLengthPercentage) > WALL_X_MAX) {
		nextMomentum = Vec3f(-nextMomentum.x(), nextMomentum.y(), nextMomentum.z());
		nextTargetAngle = currentAngle < 180 ? 90 : 270;
		collided = true;
		stepLengthPercentage = (WALL_X_MAX - x - nearestXOffset.x()) / moveVector.x();
		//createAnimationAtCollisionPoint(Vec3f(WALL_X_MAX,y,z), collisionAnimationSize, COLLISION_WALL_NORMAL_X, diskType);
	} else if((transform.getTranslation().x() - nearestXOffset.x() + moveVector.x() * stepLengthPercentage) < WALL_X_MIN) {
		nextMomentum = Vec3f(-nextMomentum.x(), nextMomentum.y(), nextMomentum.z());
		nextTargetAngle = currentAngle < 180 ? 90 : 270;
		collided = true;
		stepLengthPercentage = (WALL_X_MIN - x + nearestXOffset.x()) / moveVector.x();
		//createAnimationAtCollisionPoint(Vec3f(WALL_X_MIN,y,z), collisionAnimationSize, COLLISION_WALL_NORMAL_X, diskType);
	}

	if ((transform.getTranslation().y() + nearestYOffset.y() + moveVector.y() * stepLengthPercentage) > WALL_Y_MAX) {
		nextMomentum = Vec3f(nextMomentum.x(), -nextMomentum.y(), nextMomentum.z());
		nextTargetAngle = currentAngle < 90 || currentAngle >= 270 ? 0 : 180;
		collided = true;
		stepLengthPercentage = (WALL_Y_MAX - y - nearestYOffset.y()) / moveVector.y();
		//createAnimationAtCollisionPoint(Vec3f(x,WALL_Y_MAX,z), collisionAnimationSize, COLLISION_WALL_NORMAL_Y, diskType);
	} else if((transform.getTranslation().y() - nearestYOffset.y() + moveVector.y() * stepLengthPercentage) < WALL_Y_MIN) {
		nextMomentum = Vec3f(nextMomentum.x(), -nextMomentum.y(), nextMomentum.z());
		nextTargetAngle = currentAngle < 90 || currentAngle >= 270 ? 0 : 180;
		collided = true;
		stepLengthPercentage = (WALL_Y_MIN - y + nearestYOffset.y()) / moveVector.y();
		//createAnimationAtCollisionPoint(Vec3f(x,WALL_Y_MIN,z), collisionAnimationSize, COLLISION_WALL_NORMAL_Y, diskType);
	}
	
	if ((transform.getTranslation().z() + nearestZOffset.z() + moveVector.z() * stepLengthPercentage) > WALL_Z_MAX) {
		nextMomentum = Vec3f(nextMomentum.x(), nextMomentum.y(), -nextMomentum.z());
		nextTargetAngle = currentAngle < 180 ? 90 : 270;
		collided = true;
		stepLengthPercentage = (WALL_Z_MAX - z - nearestZOffset.z()) / moveVector.z();
		//createAnimationAtCollisionPoint(Vec3f(x,y,WALL_Z_MAX), collisionAnimationSize, COLLISION_WALL_NORMAL_Z, diskType);
		if (diskType != userFaction) {
			state = DISK_STATE_RETURNING;
			std::cout << "enemy disk returning" << '\n';
		} else {
			if (state == DISK_STATE_RETURNING) {
				catchDisk();
				handler->handleDiskCatch();
			}
		}
	} else if((transform.getTranslation().z() - nearestZOffset.z() + moveVector.z() * stepLengthPercentage) < WALL_Z_MIN) {
		nextMomentum = Vec3f(nextMomentum.x(), nextMomentum.y(), -nextMomentum.z());
		nextTargetAngle = currentAngle < 180 ? 90 : 270;
		collided = true;
		stepLengthPercentage = (WALL_Z_MIN - z + nearestZOffset.z()) / moveVector.z();
		//createAnimationAtCollisionPoint(Vec3f(x,y,WALL_Z_MIN), collisionAnimationSize, COLLISION_WALL_NORMAL_Z, diskType);
		if (diskType == userFaction) {
			state = DISK_STATE_RETURNING;
			std::cout << "player disk returning" << '\n';
		} else {
			if (state == DISK_STATE_RETURNING) {
				catchDisk();
				handler->handleDiskCatch();
			}
		}
	}

	// making sure disk won't get into walls at all...
	Vec3f newPosition = transform.getTranslation() + moveVector * stepLengthPercentage;
	newPosition = Vec3f(
		tg_math::min(tg_math::max(newPosition.x(), WALL_X_MIN + nearestXOffset.x()), WALL_X_MAX - nearestXOffset.x()),
		tg_math::min(tg_math::max(newPosition.y(), WALL_Y_MIN + nearestXOffset.y()), WALL_Y_MAX - nearestXOffset.y()),
		tg_math::min(tg_math::max(newPosition.z(), WALL_Z_MIN + nearestXOffset.z()), WALL_Z_MAX - nearestXOffset.z())
		);
	transform.setTranslation(newPosition);
	if (collided) {
		lastCollisionAngle = targetAngle;
		targetAngle = nextTargetAngle;
		momentum = nextMomentum;
		lastCollisionTime = elapsedTime;
	}
}

bool Disk::collidesWithEnemyShield() {
	// simple radius detection
	Vec3f shieldToDisk = getPosition() - enemyShield->getPosition();
	Real32 addedRadiuses = enemyShield->getRadius() + diskRadius;
	Real32 addedRadiusesSquared = addedRadiuses * addedRadiuses;
	if (shieldToDisk.squareLength() < addedRadiusesSquared) {
		// more complex circle plane collision detection
		Vec3f shieldNormal;
		enemyShield->getRotation().multVec(Vec3f(0,1,0), shieldNormal);
		Real32 angleBetweenNormals = currentAxis.enclosedAngle(shieldNormal);
		Vec3f intersectionLineDirection = currentAxis.cross(shieldNormal);
		Vec3f shieldToIntersectionLineDirection = intersectionLineDirection.cross(shieldNormal);
		if (shieldToIntersectionLineDirection.dot(currentAxis) == 0) {
			// the circles lie in the same plane
			return false;
		} else {
			Vec3f shieldToIntersectionLine = shieldToIntersectionLineDirection * (shieldToDisk.dot(currentAxis)/shieldToIntersectionLineDirection.dot(currentAxis));
			Quaternion shieldProjectionRotation(intersectionLineDirection, angleBetweenNormals);
			Vec3f shieldProjectedMiddlePointOffset;
			shieldProjectionRotation.multVec(shieldToIntersectionLine * (-1), shieldProjectedMiddlePointOffset);
			Vec3f shieldProjectedPosition = enemyShield->getPosition() + shieldToIntersectionLine + shieldProjectedMiddlePointOffset;
			if ((shieldProjectedPosition - getPosition()).squareLength() < addedRadiusesSquared) {
				return true;
			}
		}
	}
	return false;
}

Quaternion interpolateVector(Vec3f vec1, Vec3f vec2, Real32 factor) {
	Vec3f rotationAxis = vec1.cross(vec2);
	Real32 angleToRotate = tg_math::rad2Degree(vec2.enclosedAngle(vec1));
	//Real32 angleToRotate = osgACos(momentum.dot(directionToInterpolateTo) / (momentum.length() * directionToInterpolateTo.length()));
	// multiplies the quaternion with momentum and saves the result in momentum
	return Quaternion(rotationAxis, tg_math::degree2Rad(angleToRotate * factor));
}