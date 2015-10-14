#include "Bullet.h"
#include "Entity.h"

Bullet::Bullet(float xPos, float yPos, Entity* obj) {
	x = xPos;
	y = yPos;
	velocity = BULLET_VEL;
	object = obj;
	alive = true;

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
void Bullet::dies() { alive = false; }
bool Bullet::isAlive() const { return alive; }
float Bullet::getxPos() const { return x; }
float Bullet::getyPos() const { return y; }
float Bullet::getVelocity() const { return velocity; }
bool Bullet::collidesWith(Entity* target) {
	if (target->getType() == ENEMY && object->getType() == PLAYER) {
		return (y + HEIGHT > target->getYPos() - (target->getHeight()/2)) && ((x + WIDTH > target->getxPos() - (target->getWidth() / 2)) && ((x - WIDTH < target->getxPos() + (target->getWidth() / 2))));
	}
	else if (target->getType() == PLAYER && object->getType() == ENEMY) {
		return (y - HEIGHT > target->getYPos() + (target->getHeight() / 2)) && ((x + WIDTH > target->getxPos() - (target->getWidth() / 2)) && ((x - WIDTH < target->getxPos() + (target->getWidth() / 2))));
	}
}