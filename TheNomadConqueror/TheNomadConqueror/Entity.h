#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once

#include "SheetSprite.h"
#include <string>
#include <iostream>	
#include <vector>

enum TYPE { PLAYER, GHOST, SPIDER, FISH, SLIME, T_BLOCK, B_BLOCK, L_BLOCK, R_BLOCK, TL_BLOCK, TR_BLOCK, KEY_BLOCK, SPIKES, TINY_RAY_GUN, GOLD_KEY, COIN };
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
	float getWidth() const;
	float getHeight() const;
	bool isStatic() const;
	bool isInAir() const;
	bool checkAlive() const;
	TYPE getType() const;
	bool willCollide(float distance) const;
	bool collidesWith(Entity* object) const;
	SheetSprite& getMainSprite();
	
	void changeStatic(bool newStatic);
	void changeInAir(bool newInAir);
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
		 float dir;
		 TYPE type;
		 bool pstatic;
		 bool inAir;
		 bool alive;

		 SheetSprite staticSprite;
		 SheetSprite currentSprite;
};