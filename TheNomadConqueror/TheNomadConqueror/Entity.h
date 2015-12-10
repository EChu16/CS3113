#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once

#include "SheetSprite.h"
#include <string>
#include <iostream>	
#include <vector>

enum TYPE { PLAYER, GHOST, SPIDER, FISH, SLIME, BLOCK, HALF_BLOCK, TL_TRI_BLOCK, TR_TRI_BLOCK, GOLDKEY_BLOCK, SPIKES, TINY_RAY_GUN, GOLD_KEY, COIN, WATER, WATERTOP };
class Entity {
public:
	Entity(float xPos, float yPos, float dir, TYPE entityType);
	float getXPos() const;
	float getYPos() const;
	float getXVel() const;
	float getYVel() const;
	float getXAcc() const;
	float getYAcc() const;
	float getDirection() const;
	float getWidthFromCenter() const;
	float getHeightFromCenter() const;
	bool isStatic() const;
	bool isOnSurface() const;
	bool checkAlive() const;
	TYPE getType() const;
	bool rightCollision(Entity* solid) const;
	bool leftCollision(Entity* solid) const;
	bool aboveCollision(Entity* solid) const;
	bool belowCollision(Entity* solid) const;
	bool collidesWith(Entity* solid) const;
	bool collidedAction(Entity* solid);
	bool hasGoldKey() const;
	bool hasTinyGun() const;
	SheetSprite& getMainSprite();
	
	void setMainSprite(SheetSprite& mainSprite);
	void changeStatic(bool newStatic);
	void changeOnSurface(bool newInAir);
	void changeXVel(float newX);
	void changeYVel(float newY);
	void xTranslate(float shiftX);
	void yTranslate(float shiftY);
	void changeDirection();
	void Update(float elapsed);
	void dies();
	void Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite);

private: float x;
		 float y;
		 float xVel;
		 float yVel;
		 float xAcc;
		 float yAcc;
		 float widthFromCenter;
		 float heightFromCenter;
		 float dir;
		 int coins;
		 TYPE type;
		 bool pstatic;
		 bool onSurface;
		 bool alive;
		 std::vector<Entity> items;
		 SheetSprite staticSprite;
		 SheetSprite currentSprite;
};