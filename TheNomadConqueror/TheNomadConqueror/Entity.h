#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once

#include "SheetSprite.h"
#include <string>
#include <iostream>	
#include <vector>

enum TYPE { PLAYER, GHOST, SPIDER, FISH, SLIME, T_BLOCK, BLOCK, L_BLOCK, R_BLOCK, TL_BLOCK, TL_TRI_BLOCK, TR_TRI_BLOCK, TR_BLOCK, TL_HALF_BLOCK,
	T_HALF_BLOCK, TR_HALF_BLOCK, ITEM_BLOCK, TR_CURVE_BLOCK, TL_CURVE_BLOCK, CRATE, GOLDKEY_BLOCK, SPIKES, TINY_RAY_GUN, GOLD_KEY, COIN, WATER, WATERTOP };
class Entity {
public:
	Entity(float xPos, float yPos, float dir, TYPE entityType, SheetSprite& mainSprite);
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
	bool collidesWith(Entity* solid) const;
	void collidedAction(Entity* solid);
	SheetSprite& getMainSprite();
	
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
		 TYPE type;
		 bool pstatic;
		 bool onSurface;
		 bool alive;

		 SheetSprite staticSprite;
		 SheetSprite currentSprite;
};