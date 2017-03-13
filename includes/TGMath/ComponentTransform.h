#ifndef _ComponentTransform_H_
#define _ComponentTransform_H_

#include "TGMath/MathTypes.h"
#include "TGMath/Vector.h"
#include "TGMath/Quaternion.h"

class ComponentTransform {
public:
	ComponentTransform();
	ComponentTransform(const ComponentTransform &obj);

	void setTranslation(Vec3f translation);
	void setScale(Vec3f scale);
	void setRotation(Quaternion rotation);
	
	Vec3f getTranslation() const;
	Vec3f getScale() const;
	Quaternion getRotation() const;
private:
	Vec3f _translation;
	Vec3f _scale;
	Quaternion _rotation;
};

#endif