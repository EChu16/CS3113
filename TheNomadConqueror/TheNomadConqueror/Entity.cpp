#include "Entity.h"

Entity::Entity(float xPos, float yPos, float startingDir, TYPE entityType, SheetSprite& mainSprite) : x(xPos), y(yPos), dir(startingDir), type(entityType), isStatic(true), inAir(false), alive(true){
	if (entityType == PLAYER) {
		xVel = 20.0f;
		yVel = 20.0f;
		xAcc = 0;
		yAcc = 0;
		staticSprite = mainSprite;
		currentSprite = mainSprite;
	}
}

float Entity::getXPos() const{ return x; }
float Entity::getYPos() const{ return y; }
float Entity::getXVel() const { return xVel; }
float Entity::getYVel() const { return yVel; }
float Entity::getXAcc() const { return xAcc; }
float Entity::getYAcc() const { return yAcc; }
float Entity::getDirection() const { return dir; }

SheetSprite& Entity::getMainSprite() { return staticSprite; }
bool Entity::checkAlive() const{ return alive; }
TYPE Entity::getType() const{ return type; }
bool Entity::willCollide(float distance) const{ return (x + distance < -2.7f) || (x - distance > 2.7f); }
//float Entity::getWidth() const{ return width; }
//float Entity::getHeight() const{ return height; }
//bool Entity::collidesWith(Entity* object) const{
//	return ((y + (height / 2)) >= (object->getYPos() - (object->getHeight() / 2))) &&
//		(((x + (width / 2)) >= (object->getxPos() - (object->getWidth() / 2))) ||
//		((x - (width / 2)) <= (object->getxPos() + (object->getWidth() / 2))));
//}
void Entity::Update(float elapsed)
{
	x += xVel * elapsed;
	y += yVel * elapsed;
}
void Entity::xTranslate(float shiftX) { x += shiftX; }
void Entity::yTranslate(float shiftY) { y += shiftY; }
void Entity::changeDirection() { dir *= -1; }
void Entity::dies() { alive = false; }
void Entity::Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite) {
	sprite.Draw(program, gameMatrix, x, y);
}