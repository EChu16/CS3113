#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once

#include "SheetSprite.h"
#include <string>
#include <iostream>	
#include <vector>

enum TYPE { PLAYER, GHOST, SPIDER, FISH, BLOCK, HALF_BLOCK, TL_TRI_BLOCK, TR_TRI_BLOCK, GOLDKEY_BLOCK, SPIKE, TINY_RAY_GUN, GOLD_KEY, COIN, WATER, REG_BULLET, BIG_BULLET, ENEMY_BULLET };
class Entity {
public:
	Entity(float xPos, float yPos, float dir, TYPE entityType);
	float getHp() const;
	float getXPos() const;
	float getYPos() const;
	float getXVel() const;
	float getYVel() const;
	float getXAcc() const;
	float getYAcc() const;
	float getDirection() const;
	float getWidthFromCenter() const;
	float getHeightFromCenter() const;
	float getTimeAlive() const;
	float getExpireTime() const;
	float getStallShootTime() const;
	float getLastBulletTime() const;
	float getSinceLastExpTime() const;
	bool canDoubleJump() const;
	bool isStatic() const;
	bool isOnSurface() const;
	bool isAlive() const;
	bool normExp() const;
	TYPE getType() const; 
	SheetSprite& getMainSprite();
	SheetSprite& getCurrentSprite();
	bool hasGoldKey() const;
	bool hasTinyGun() const;
	bool rightCollision(Entity* solid) const;
	bool leftCollision(Entity* solid) const;
	bool aboveCollision(Entity* solid) const;
	bool belowCollision(Entity* solid) const;
	bool collidesWith(Entity* solid) const;
	bool collidedAction(Entity* solid);

	void setMainSprite(SheetSprite& mainSprite);
	void setCurrentSprite(SheetSprite& currSprite, float theExpTime);
	bool decreaseHP(float damageTaken);
	void incExpTime(float incTime);
	void setImmX(float immX);
	void setImmY(float immY);
	void respawn();
	void changeStatic(bool newStatic);
	void changeOnSurface(bool newInAir);
	void resetLastBulletTime();
	void xTranslate(float shiftX);
	void yTranslate(float shiftY);
	void changeXVel(float newX);
	void changeYVel(float newY);
	void changeDirection(float newDir);
	void dies();
	void animate(float elapsed);
	void Update(float elapsed);
	void Draw(ShaderProgram* program, Matrix& gameMatrix);

private: float x;
		 float y;
		 float xVel;
		 float yVel;
		 float xAcc;
		 float yAcc;
		 float widthFromCenter;
		 float heightFromCenter;
		 float dir;
		 float originalX;
		 float originalY;
		 float originalDir;
		 float hp;
		 float sinceChangeExpTime;
		 float expTime;
		 int coins;
		 float sinceLastBullet;
		 float stallShootBullet;
		 float expireTime;
		 float timeAlive;
		 int randNum;
		 TYPE type;
		 bool pstatic;
		 bool onSurface;
		 bool alive;
		 bool goldKey;
		 bool doubleJump;
		 bool changedExp;
		 std::vector<Entity*> items;
		 SheetSprite staticSprite;
		 SheetSprite currentSprite;
};