
#ifndef _Player_H_
#define _Player_H_

#include "Common.h"
#include "Disk.h"
#include "Shield.h"

class Player : public DiskEventHandler {
public:
	Player* getEnemy();
	void setEnemy(Player* newEnemy);
	Vec3f getTorsoPosition();
	Quaternion getHeadRotation();
	void setHeadRotation(Quaternion rotation);
	Vec3f getHeadPosition();
	void setHeadPosition(Vec3f newPosition);
	Quaternion getDiskArmRotation();
	void setDiskArmRotation(Quaternion rotation);
	Vec3f getDiskArmPosition();
	void setDiskArmPosition(Vec3f newPosition);
	Quaternion getShieldArmRotation();
	void setShieldArmRotation(Quaternion rotation);
	Vec3f getShieldArmPosition();
	void setShieldArmPosition(Vec3f newPosition);
	PlayerFaction getFaction();
	Disk* getDisk();
	Shield* getShield();
	void setLifeCount(Int32 count);
	void handleDiskCatch();
	Player(PlayerFaction faction);
	~Player();
	void update();
private:
	void recalculatePositions();
	void loseLife();
	Int32 life;
	PlayerFaction faction;
	Player* enemy;
	Disk* disk;
	Shield* shield;
	ComponentTransform torsoTransform;
	ComponentTransform headTransform;
	ComponentTransform diskArmTransform;
	ComponentTransform shieldArmTransform;
	Vec3f torsoPosition;
	Quaternion headRotation;
	Vec3f headPosition;
	Quaternion diskArmRotation;
	Vec3f diskArmPosition;
	Quaternion shieldArmRotation;
	Vec3f shieldArmPosition;
};

#endif