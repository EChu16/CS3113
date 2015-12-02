#include "Entity.h"

Entity::Entity(float xPos, float yPos, float h, float w, float entityVelocity, TYPE entityType){
	x = xPos;
	y = yPos;
	height = h;
	width = w;
	type = entityType;
	velocity = entityVelocity;
	alive = true;
	direction = 1.0f;
}

float Entity::getxPos() const{ return x; }
float Entity::getYPos() const{ return y; }
float Entity::getWidth() const{ return width; }
float Entity::getHeight() const{ return height; }
float Entity::getVelocity() const { return velocity; }
float Entity::getDirection() const { return direction; }
bool Entity::checkAlive() const{ return alive; }
TYPE Entity::getType() const{ return type; }
bool Entity::willCollide(float distance) const{ return (x + distance < -2.7f) || (x - distance > 2.7f); }
bool Entity::collidesWith(Entity* object) const{
	return ((y + (height / 2)) >= (object->getYPos() - (object->getHeight() / 2))) &&
		(((x + (width / 2)) >= (object->getxPos() - (object->getWidth() / 2))) ||
		((x - (width / 2)) <= (object->getxPos() + (object->getWidth() / 2))));
}

void Entity::xTranslate(float shiftX) { x += shiftX; }
void Entity::yTranslate(float shiftY) { y += shiftY; }
void Entity::changeDirection() { direction *= -1; }
void Entity::dies() { alive = false; }
void Entity::Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite) {
	sprite.Draw(program, gameMatrix, x, y);
}