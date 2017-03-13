#include "TGMath/ComponentTransform.h"

ComponentTransform::ComponentTransform() {}

ComponentTransform::ComponentTransform(const ComponentTransform &obj) {
	_translation = Vec3f(obj.getTranslation());
	_scale = Vec3f(obj.getScale());
	_rotation = Quaternion(obj.getRotation());
}

void ComponentTransform::setTranslation(Vec3f translation) {
	_translation = translation;
}

void ComponentTransform::setScale(Vec3f scale) {
	_scale = scale;
}

void ComponentTransform::setRotation(Quaternion rotation) {
	_rotation = rotation;
}

Vec3f ComponentTransform::getTranslation() const {
	return _translation;
}

Vec3f ComponentTransform::getScale() const {
	return _scale;
}

Quaternion ComponentTransform::getRotation() const {
	return _rotation;
}
