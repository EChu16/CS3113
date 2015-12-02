#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once

#include "SheetSprite.h"
#include <string>
#include <iostream>	
#include <vector>

enum TYPE { PLAYER, ENEMY };
class Entity {
public:
	Entity(float xPos, float yPos, float h, float w, float entityVelocity, TYPE entityType);
	float getxPos() const;
	float getYPos() const;
	float getWidth() const;
	float getHeight() const;
	float getVelocity() const;
	float getDirection() const;
	bool checkAlive() const;
	TYPE getType() const;
	bool willCollide(float distance) const;
	bool collidesWith(Entity* object) const;

	void xTranslate(float shiftX);
	void yTranslate(float shiftY);
	void changeDirection();
	void dies();
	void Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite);

private: float x;
		 float y;
		 TYPE type;
		 bool alive;
		 float height;
		 float width;
		 float velocity;
		 float direction;
};