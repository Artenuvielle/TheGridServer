
#ifndef _Disk_H_
#define _Disk_H_

#include "Observer.h"
#include "NetworkPackets.h"
#include "Common.h"
#include "Shield.h"

class DiskEventHandler {
public:
	virtual void handleDiskCatch() = 0;
};

class Disk : public Observable<GameNotifications> {
public:
	bool setPosition(Vec3f newPosition);
	Vec3f getPosition();
	void setTargetOwnerPosition(Vec3f newPosition);
	Vec3f getTargetOwnerPosition();
	void setTargetEnemyPosition(Vec3f newPosition);
	Vec3f getTargetEnemyPosition();
	bool setRotation(Quaternion newRotation);
	void setEnemyShield(Shield* enemyShield);
	Quaternion getRotation();
	DiskState getState();
	Vec3f getMomentum();

	bool startDraw(Vec3f pos);
	bool endDraw(Vec3f pos);
	bool forceReturn();
	bool forceThrow(Vec3f pos, Vec3f momentum);
	void catchDisk();
	void update();
	Disk(PlayerFaction type, DiskEventHandler* handler);
	~Disk();
private:
	void moveDiskAtLeastUntilWallCollision(Real32 deltaTime);
	Vec3f calculateMovement(Real32 deltaTime);
	bool collidesWithEnemyShield();
	DiskEventHandler* handler;
	Vec3f momentum;
	Vec3f targetOwnerPosition;
	Vec3f targetEnemyPosition;
	Vec3f currentAxis;
	Real32 lastCollisionAngle;
	Real32 currentAngle;
	Real32 targetAngle;
	Real32 lastCollisionTime;
	Real32 axialRotationPerMillisecond;
	Real32 rotationAroundAxis;
	PlayerFaction diskType;
	Vec3f lastPositionWhileDrawn;
	Vec3f lastFowardVectorWhileDrawn;
	ComponentTransform transform;
	Real32 lastPositionUpdateTime;
	Real32 lastRotationUpdateTime;
	Shield* enemyShield;
	DiskState state;
};

#endif