#include "Shield.h"

Quaternion interpolateVector(Vec3f vec1, Vec3f vec2, Real32 factor);

Shield::Shield(PlayerFaction faction) {
	refillCharges();
}

Shield::~Shield() {}

void Shield::update(Vec3f enemyDiskPosition) {
	Real32 distance = (enemyDiskPosition - getPosition()).length();
	if (distance < shieldGrowStartDistance) {
		if (distance < shieldGrowEndDistance) {
			setRadius(shieldMaximumRadius);
		} else {
			Real32 distanceScale = (1 + cos((distance - shieldGrowEndDistance) / (shieldGrowStartDistance - shieldGrowEndDistance) * tg_math::Pi)) / 2;
			setRadius(shieldMinimumRadius + (shieldMaximumRadius - shieldMinimumRadius) * distanceScale);
		}
	} else {
		setRadius(shieldMinimumRadius);
	}
}

void Shield::setPosition(Vec3f newPosition) {
	transform.setTranslation(newPosition);
}

Vec3f Shield::getPosition() {
	return transform.getTranslation();
}

void Shield::setRotation(Quaternion newRotation) {
	transform.setRotation(newRotation);
}

Quaternion Shield::getRotation() {
	return transform.getRotation();
}

void Shield::setRadius(Real32 newRadius) {
	if (charges == 0 && newRadius >= getRadius()) {
		return;
	}
	Real32 clampedRadius = tg_math::max(shieldMinimumRadius, tg_math::min(shieldMaximumRadius, newRadius));
	Real32 newScale = clampedRadius / shieldMaximumRadius;
	transform.setScale(Vec3f(newScale, 1, newScale));
}

Real32 Shield::getRadius() {
	return transform.getScale().x() * shieldMaximumRadius;
}

void Shield::reduceCharges() {
	if (charges > 0) {
		charges--;
		notify(GAME_NOTIFICATION_PLAYER_CHANGED_SHIELD_CHARGE);
	}
}

void Shield::refillCharges() {
	charges = shieldMaximumCharges;
	notify(GAME_NOTIFICATION_PLAYER_CHANGED_SHIELD_CHARGE);
}

bool Shield::hasCharges() {
	return charges > 0;
}

Int32 Shield::getCharges() {
	return charges;
}