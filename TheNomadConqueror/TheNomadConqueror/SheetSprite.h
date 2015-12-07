#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include "ShaderProgram.h"

class SheetSprite {
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);
	SheetSprite& operator= (const SheetSprite& spriteSource);
	void Draw(ShaderProgram* program, Matrix& gameMatrix, const float shiftX, const float shiftY);

	float size;
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
};