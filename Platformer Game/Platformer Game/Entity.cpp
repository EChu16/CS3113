#include "Entity.h"

Entity::Entity(float xPos, float yPos, float h, float w, std::string etype) {
	x = xPos;
	y = yPos;
	height = h;
	width = w;
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f; 
	friction = 1.0f;
	direction = 1.0f;
	isStatic = true;
	if (etype == "start") {
		type = PLAYER;
	}
	else if (etype == "block") {
		type = BLOCK;
	}
}

float Entity::getxPos() const{ return x; }
float Entity::getYPos() const{ return y; }
float Entity::getWidth() const{ return width; }
float Entity::getHeight() const{ return height; }
float Entity::getxVelocity() const { return velocity_x; }
float Entity::getyVelocity() const { return velocity_y; }
float Entity::getDirection() const { return direction; }
TYPE Entity::getType() const { return type; }
bool Entity::willCollide(float distance) const{ return (x + distance < -2.7f) || (x - distance > 2.7f); }

void Entity::changeXAcc(float acc) { acceleration_x = acc; }
void Entity::changeXVel(float vel) { velocity_x = vel; }
void Entity::updateVals(float elapsed) {
	velocity_x += acceleration_x * elapsed;
	if (velocity_x > 0.0)
		velocity_x += -friction * elapsed;
	else if (velocity_x < 0.0)
		velocity_x += friction * elapsed;
	velocity_y += acceleration_y * elapsed;
	x += velocity_x * elapsed;
	y += velocity_y * elapsed;
}
void Entity::xTranslate(float shiftX) { x += shiftX; }
void Entity::yTranslate(float shiftY) { y += shiftY; }
void Entity::changeDirection() { direction *= -1; }
void Entity::Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite) {
	sprite.Draw(program, gameMatrix, this->x, this->y);
}