#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once

#include "SheetSprite.h"
#include <string>
#include <iostream>
#define WIDTH 0.1
#define HEIGHT 0.2
#define BULLET_VEL 2.0f

class Entity;
class Bullet {
	public: Bullet(float xPos, float yPos, Entity* obj);
			void Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite sprite);
			void yTranslate(float amount);
			void dies();

			Entity* getObject() const;
			bool isAlive() const;
			float getxPos() const;
			float getyPos() const;
			float getVelocity() const;
			bool collidesWith(Entity* target);
	private: float x;
			 float y;
			 float velocity;
			 Entity* object;
			 bool alive;
};