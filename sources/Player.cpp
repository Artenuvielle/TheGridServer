#include "Player.h"

#include <iostream>

#include "Common.h"

Player::Player(PlayerFaction faction) : faction(faction) {
	enemy = nullptr;
	headRotation = Quaternion();
	headPosition = Vec3f(0.0,170.0,30.0);
	diskArmPosition = Vec3f(20.0,130.0,50.0);
	shieldArmPosition = Vec3f(-20.0,130.0,50.0);

	life = lifeCounterMaxLife;

	disk = new Disk(faction, this);
	shield = new Shield(faction);
	disk->attach(this);
	shield->attach(this);

	torsoTransform.setScale(Vec3f(PLAYER_GEOMETRY_SCALE, PLAYER_GEOMETRY_SCALE, PLAYER_GEOMETRY_SCALE) * 1.3);
	headTransform.setScale(Vec3f(PLAYER_GEOMETRY_SCALE, PLAYER_GEOMETRY_SCALE, PLAYER_GEOMETRY_SCALE));
	diskArmTransform.setScale(Vec3f(PLAYER_GEOMETRY_SCALE, PLAYER_GEOMETRY_SCALE, PLAYER_GEOMETRY_SCALE));
	shieldArmTransform.setScale(Vec3f(PLAYER_GEOMETRY_SCALE, PLAYER_GEOMETRY_SCALE, PLAYER_GEOMETRY_SCALE));

	recalculatePositions();
}

Player::~Player() {
	delete disk;
	delete shield;
}

void Player::update() {
	recalculatePositions();
	Vec3f diskArmUp, shieldArmUp;
	diskArmRotation.multVec(Vec3f(0.0,1.0,0.0), diskArmUp);
	shieldArmRotation.multVec(Vec3f(0.0,1.0,0.0), shieldArmUp);

	shield->setPosition(shieldArmPosition + shieldArmUp * 6.5);
	shield->setRotation(shieldArmRotation);
	shield->update(enemy->getDisk()->getPosition());

	disk->setPosition(diskArmPosition + diskArmUp * 6.5);
	disk->setRotation(diskArmRotation);
	disk->setTargetOwnerPosition(getTorsoPosition());
	disk->setTargetEnemyPosition(enemy->getTorsoPosition());
	disk->update();

	if (disk->getState() == DISK_STATE_RETURNING && (disk->getPosition() - diskArmPosition).squareLength() < (diskRadius + 10) * (diskRadius + 10)) {
		disk->catchDisk();
		shield->refillCharges();
	}

	if (enemy->getDisk()->getState() == DISK_STATE_FREE_FLY && (Vec3f(enemy->getDisk()->getPosition().x(), 0.0, enemy->getDisk()->getPosition().z()) - Vec3f(getTorsoPosition().x(), 0.0, getTorsoPosition().z())).squareLength() < (diskRadius + PLAYER_HEAD_SIZE) * (diskRadius + PLAYER_HEAD_SIZE)) {
		if (enemy->getDisk()->getPosition().y() < getHeadPosition().y()) {
			std::cout << "A player got hit by a disk" << '\n';
			enemy->getDisk()->forceReturn();
			loseLife();
		}
	}
};

void Player::loseLife() {
	if (gameRunning) {
		life -= 1;
		if (life == 0) {
			gameRunning = false;
			std::cout << "Game over" << std::endl;
		}
		notify(GAME_NOTIFICATION_PLAYER_CHANGED_LIFE);
	}
}

void Player::recalculatePositions() {
	Vec3f headYAxisDirection;
	headRotation.multVec(Vec3f(0.0,1.0,0.0), headYAxisDirection);
	torsoPosition = headPosition - headYAxisDirection * PLAYER_HEAD_SIZE - Vec3f(0.0,PLAYER_TORSO_HEAD_OFFSET,0.0);
    
	torsoTransform.setTranslation(torsoPosition);
	Vec3f headEulerAxisRotation;
	headRotation.getEulerAngleRad(headEulerAxisRotation);
	torsoTransform.setRotation(Quaternion(Vec3f(0.0,1.0,0.0), headEulerAxisRotation.y()));
	headTransform.setTranslation(headPosition);
	headTransform.setRotation(headRotation);
	Vec3f diskArmForward, shieldArmForward;
	diskArmRotation.multVec(Vec3f(0.0,0.0,1.0), diskArmForward);
	shieldArmRotation.multVec(Vec3f(0.0,0.0,1.0), shieldArmForward);
	diskArmTransform.setTranslation(diskArmPosition - diskArmForward * 7.5);
	diskArmTransform.setRotation(diskArmRotation);
	shieldArmTransform.setTranslation(shieldArmPosition - shieldArmForward * 7.5);
	shieldArmTransform.setRotation(shieldArmRotation);
}

PlayerFaction Player::getFaction() {
	return faction;
}

Player* Player::getEnemy() {
	return enemy;
}

void Player::setEnemy(Player* newEnemy) {
	enemy = newEnemy;
	disk->setEnemyShield(enemy->getShield());
}

Vec3f Player::getTorsoPosition() {
	return torsoPosition;
}

Quaternion Player::getHeadRotation() {
	return headRotation;
}

void Player::setHeadRotation(Quaternion rotation) {
	headRotation = rotation;
}

Vec3f Player::getHeadPosition() {
	return headPosition;
}

void Player::setHeadPosition(Vec3f newPosition) {
	headPosition = newPosition;
}

Quaternion Player::getDiskArmRotation() {
	return diskArmRotation;
}

void Player::setDiskArmRotation(Quaternion rotation) {
	diskArmRotation = rotation;
}

Vec3f Player::getDiskArmPosition() {
	return diskArmPosition;
}

void Player::setDiskArmPosition(Vec3f newPosition) {
	diskArmPosition = newPosition;
}

Quaternion Player::getShieldArmRotation() {
	return shieldArmRotation;
}

void Player::setShieldArmRotation(Quaternion rotation) {
	shieldArmRotation = rotation;
}

Vec3f Player::getShieldArmPosition() {
	return shieldArmPosition;
}

void Player::setShieldArmPosition(Vec3f newPosition) {
	shieldArmPosition = newPosition;
}

Disk* Player::getDisk() {
	return disk;
}

Shield* Player::getShield() {
	return shield;
}

void Player::setLifeCount(Int32 count) {
	life = count;
	notify(GAME_NOTIFICATION_PLAYER_CHANGED_LIFE);
}

Int32 Player::getLifeCount() {
	return life;
}

void Player::handleDiskCatch() {
	loseLife();
}
