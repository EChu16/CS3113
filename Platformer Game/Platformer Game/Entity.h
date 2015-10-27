#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once

#include "SheetSprite.h"
#include <string>
#include <iostream>	
#include <vector>
enum TYPE { PLAYER, COIN };
class Entity {
	public:
		Entity(float xPos, float yPos, float h, float w, std::string etype);
		float getxPos() const;
		float getYPos() const;
		float getWidth() const;
		float getHeight() const;
		float getxVelocity() const;
		float getyVelocity() const;
		float getDirection() const;
		TYPE getType() const;
		bool willCollide(float distance) const;

		void xTranslate(float shiftX);
		void yTranslate(float shiftY);
		void changeDirection();
		void Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite);

	private: float x;
			 float y;
			 float height;
			 float width;
			 float velocity_x;
			 float velocity_y;
			 float acceleration_x;
			 float acceleration_y;
			 bool isStatic;
			 float direction;
			 TYPE type;
};