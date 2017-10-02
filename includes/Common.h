
#ifndef _Common_H_
#define _Common_H_

#include "TGMath.h"

enum PlayerFaction {
	PLAYER_FACTION_BLUE = 0,
	PLAYER_FACTION_ORANGE
};

const PlayerFaction userFaction = PLAYER_FACTION_BLUE;
const PlayerFaction enemyFaction = (userFaction == PLAYER_FACTION_BLUE ? PLAYER_FACTION_ORANGE : PLAYER_FACTION_BLUE);

const float WALL_RIGHT_MAX = 135;
const float WALL_RIGHT_MIN = -135;
const float WALL_UP_MAX = 270;
const float WALL_UP_MIN = 0;
const float WALL_BACKWARD_MAX = 135;
const float WALL_BACKWARD_MIN = -945;

const float WALL_RIGHT_MID = (WALL_RIGHT_MAX + WALL_RIGHT_MIN) / 2;
const float WALL_UP_MID = (WALL_UP_MAX + WALL_UP_MIN) / 2;
const float WALL_BACKWARD_MID = (WALL_BACKWARD_MAX + WALL_BACKWARD_MIN) / 2;
const float WALL_BACKWARD_DIFF = WALL_BACKWARD_MAX - WALL_BACKWARD_MIN;

const Real32 PLAYER_HEAD_SIZE = 15;
const Real32 PLAYER_TORSO_HEAD_OFFSET = 10;
const Real32 PLAYER_GEOMETRY_SCALE = 2;

const Int32 lifeCounterMaxLife = 4;
const Real32 lifeCounterMaxTransparency = 0.3f;
const Real32 lifeCounterTransparencyChangeTime = 0.2f; // s

const Real32 diskRadius = 15; // cm
const Real32 diskHeight = 3; // cm
const Real32 diskSpeed = 0.5f; // cm/ms
const Real32 diskAxisRotationFactor = 100;
const Real32 diskRotationTimeAfterCollision = 0.3f; // s
const Real32 diskEnemyMomentumAttractionFactor = 1.8f; // deg/sec
const Real32 diskOwnerMomentumAttractionFactor = 2.0f; // deg/sec
const Real32 diskMinimalAxialRotationAfterCollision = 0.0004f; // deg/millisec

const Real32 shieldMinimumRadius = 2.5; // cm
const Real32 shieldMaximumRadius = 20; // cm
const Real32 shieldGrowStartDistance = 400; // cm
const Real32 shieldGrowEndDistance = 250; // cm
const Int32 shieldMaximumCharges = 3;

const Vec3f aiDefaultHeadPositionEnemy = Vec3f(0.0,165.0,-810.0);
const Vec3f aiDefaultHeadPositionUser = Vec3f(0.0,165.0,0.0);
const Real32 aiHeadMaxSpeed = 25.f; // cm/s
const Real32 aiHeadMaxRotation = 0.8f; // rad/s
const Real32 aiArmMaxSpeed = 50.f; // cm/s
const Real32 aiArmMaxRotation = 3.5f; // rad/s
const Real32 aiMinTimeUntilAttack = 2.f; // s
const Real32 aiDefendArmTorsoDistance = 35.f; // cm
const Real32 aiCatchArmTorsoDistance = 50.f; // cm

const Real32 collisionAnimationSize = 150; // in cm
const Real32 scoreAnimationSize = 1200; // in cm

const Int32 lightTrailMaxPoints = 70;
const Real32 lightTrailPointsPerSecond = 30.f;
const Real32 lightTrailSizeGrow = 100.f;
const Real32 lightTrailMaxSize = 1.5f;
const Real32 lightTrailInputPointMinDistance = 20.f;

extern bool gameRunning;
extern Real32 elapsedTime;

enum GameNotifications {
	GAME_NOTIFICATION_PLAYER_CHANGED_LIFE = 0,
	GAME_NOTIFICATION_PLAYER_CHANGED_SHIELD_CHARGE,
	GAME_NOTIFICATION_DISK_STATE_CHANGED,
	GAME_NOTIFICATION_DISK_THROWN,
	GAME_NOTIFICATION_WALL_COLLISION
};

#endif
