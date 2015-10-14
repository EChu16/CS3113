#include "Bullet.h"

Bullet::Bullet(float xPos, float yPos, float bulletVelocity, Entity* obj) {
	x = xPos;
	y = yPos;
	velocity = bulletVelocity;
	object = obj;

	//Have the bullet start at the tip of the player/enemy
	if (object->getType() == PLAYER)
		y += 0.3f;
	else y -= 0.3f;
}

void Bullet::Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite sprite) {
	sprite.Draw(program, gameMatrix, x, y);
}

void Bullet::yTranslate(float amount) {
	if (object->getType() == PLAYER)
		y += amount;
	else y += amount * -1.0;
}

bool Bullet::getxPos() const { return x; }
bool Bullet::getyPos() const { return y; }
float Bullet::getVelocity() const { return velocity; }
Entity* Bullet::getObject() const { return object; }
bool Bullet::collidesWith(Entity* target) {
	if (object->getType() == PLAYER) {
		return (y + HEIGHT > target->getYPos() - (target->getHeight()/2)) && ((x + WIDTH > target->getxPos() - (target->getWidth()/2)) || ((x - WIDTH < target->getxPos() + (target->getWidth()/2))));
	}
	else if (object->getType() == ENEMY) {
		return (y - HEIGHT > target->getYPos() + (target->getHeight() / 2)) && ((x + WIDTH > target->getxPos() - (target->getWidth() / 2)) || ((x - WIDTH < target->getxPos() + (target->getWidth() / 2))));
	}
}