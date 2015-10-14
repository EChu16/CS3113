#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once

#include "Entity.h"
#include "SheetSprite.h"
#include <string>
#include <iostream>
#define WIDTH 0.1
#define HEIGHT 0.2

class Bullet {
public: Bullet(float xPos, float yPos, float bulletVelocity, Entity* obj);
		void Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite sprite);
		void yTranslate(float amount);

		bool getxPos() const;
		bool getyPos() const;
		bool collidesWith(Entity* target);
		float getVelocity() const;
		Entity* getObject() const;
	private:
		float x;
		float y;
		float velocity;
		Entity* object;
};