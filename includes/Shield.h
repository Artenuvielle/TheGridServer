
#ifndef _Shield_H_
#define _Shield_H_

#include "Common.h"

class Shield {
public:
	void setPosition(Vec3f newPosition);
	Vec3f getPosition();
	void setRotation(Quaternion newRotation);
	Quaternion getRotation();
	void setRadius(Real32 newRadius);
	Real32 getRadius();
	
	void reduceCharges();
	void refillCharges();
	bool hasCharges();
	void update(Vec3f enemyDiskPosition);
	Shield(PlayerFaction type);
	~Shield();
private:
	Int32 charges;
	ComponentTransform transform;
};

#endif