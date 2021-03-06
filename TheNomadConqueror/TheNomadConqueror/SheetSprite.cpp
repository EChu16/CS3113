#include "SheetSprite.h"

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

SheetSprite::SheetSprite() {
	size = textureID = u = v = width = height = 0;
}
SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size) :textureID(textureID), u(u), v(v),
width(width), height(height), size(size){}

SheetSprite& SheetSprite::operator= (const SheetSprite& spriteSource) {
	this->size = spriteSource.size;
	this->u = spriteSource.u;
	this->v = spriteSource.v;
	this->width = spriteSource.width;
	this->height = spriteSource.height;
	this->textureID = spriteSource.textureID;

	return *this;
}

bool SheetSprite::operator!= (const SheetSprite& spriteSource) {
	return (this->size != spriteSource.size || this->u != spriteSource.u || this->v != spriteSource.v || this->width != spriteSource.width
		|| this->height != spriteSource.height || this->textureID != spriteSource.textureID);
}

void SheetSprite::Draw(ShaderProgram* program, Matrix& gameMatrix, const float shiftX, const float shiftY, const float theDir){
	GLfloat texCoords[] = {
		u, v + height,
		u + width, v,
		u, v,
		u + width, v,
		u, v + height,
		u + width, v + height
	};
	float aspect = width / height;
	float vertices[] = {
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, 0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, -0.5f * size };

	gameMatrix.identity();
	gameMatrix.Translate(shiftX, shiftY, 0.0);
	gameMatrix.Scale(theDir, 1.0f, 1.0f);
	program->setModelMatrix(gameMatrix);

	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);


}