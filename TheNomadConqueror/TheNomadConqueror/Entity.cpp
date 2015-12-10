#include "Entity.h"

Entity::Entity(float xPos, float yPos, float startingDir, TYPE entityType) : x(xPos), y(yPos), dir(startingDir), type(entityType), pstatic(true), onSurface(true), alive(true){
	if (entityType == PLAYER) {
		xVel = 0.0f;
		yVel = 0.0f;
		xAcc = 0;
		yAcc = -20.0f;
		coins = 0;  
		widthFromCenter = 0.47f;
		heightFromCenter = 1.0f;
	}
	else if (entityType == BLOCK) {
		widthFromCenter = 0.5f;
		heightFromCenter = 0.5f;
	}
	else if (entityType == HALF_BLOCK) {
		widthFromCenter = 0.5f;
		heightFromCenter = 0.125f;
	}
}
float Entity::getXPos() const{ return x; }
float Entity::getYPos() const{ return y; }
float Entity::getXVel() const { return xVel; }
float Entity::getYVel() const { return yVel; }
float Entity::getXAcc() const { return xAcc; }
float Entity::getYAcc() const { return yAcc; }
float Entity::getDirection() const { return dir; }
bool Entity::isStatic() const { return pstatic; }
bool Entity::isOnSurface() const { return onSurface; }
bool Entity::hasGoldKey() const {
	for (int i = 0; i < items.size(); i) {
		if (items[i].getType() == GOLD_KEY) {
			return true;
		}
	}
	return false;
}
bool Entity::hasTinyGun() const {
	for (int i = 0; i < items.size(); i) {
		if (items[i].getType() == TINY_RAY_GUN) {
			return true;
		}
	}
	return false;
}
void Entity::changeStatic(bool newStatic) { pstatic = newStatic; }
void Entity::changeOnSurface(bool newOnSurface) { onSurface = newOnSurface; }
void Entity::changeXVel(float newX) { xVel = newX; }
void Entity::changeYVel(float newY) { yVel = newY; }
void Entity::setMainSprite(SheetSprite& mainSprite) { mainSprite = mainSprite; staticSprite = mainSprite; }
SheetSprite& Entity::getMainSprite() { return staticSprite; }
bool Entity::checkAlive() const{ return alive; }
TYPE Entity::getType() const{ return type; }
float Entity::getWidthFromCenter() const{ return widthFromCenter; }
float Entity::getHeightFromCenter() const{ return heightFromCenter; }
bool Entity::rightCollision(Entity* solid) const {
	return (this->getXPos() > solid->getXPos() )
		&& (this->getYPos() + this->getHeightFromCenter() > solid->getYPos() - solid->getHeightFromCenter())
		&& (this->getYPos() - this->getHeightFromCenter() < solid->getYPos() + solid->getHeightFromCenter())
		&& (this->dir == 1.0f);
}
bool Entity::leftCollision(Entity* solid) const {
	return (this->getXPos() < solid->getXPos())
		&& (this->getYPos() + this->getHeightFromCenter() > solid->getYPos() - solid->getHeightFromCenter())
		&& (this->getYPos() - this->getHeightFromCenter() < solid->getYPos() + solid->getHeightFromCenter())
		&& (this->dir == -1.0f);
}
bool Entity::aboveCollision(Entity* solid) const {
	return (this->getYPos() + this->getHeightFromCenter() >= solid->getYPos() - solid->getHeightFromCenter())
		&& (this->getXPos() + this->getWidthFromCenter() < solid->getXPos() - solid->getWidthFromCenter())
		&& (this->getXPos() - this->getWidthFromCenter() < solid->getXPos() + solid->getWidthFromCenter())
		&& (!this->isOnSurface());
}
bool Entity::belowCollision(Entity* solid) const {
	return (this->getYPos() - this->getHeightFromCenter() <= solid->getYPos() + solid->getHeightFromCenter())
		&& ((this->getXPos() + this->getWidthFromCenter() > solid->getXPos() + solid->getWidthFromCenter()) ||
		(this->getXPos() + this->getWidthFromCenter() < solid->getXPos() + solid->getWidthFromCenter()))
		&& (this->getXPos() - this->getWidthFromCenter() > solid->getXPos() - solid->getWidthFromCenter());
}
bool Entity::collidesWith(Entity* solid) const {
	//If there is a collision
	if ((this->getXPos() + this->getWidthFromCenter() >= solid->getXPos() - solid->getWidthFromCenter())
		&& (this->getXPos() - this->getWidthFromCenter() <= solid->getXPos() + solid->getWidthFromCenter())
		&& (this->getYPos() + this->getHeightFromCenter() >= solid->getYPos() - solid->getHeightFromCenter())
		&& (this->getYPos() - this->getHeightFromCenter() <= solid->getYPos() + solid->getHeightFromCenter())) {
		return true;
	}
	else return false;
}

bool Entity::collidedAction(Entity* solid) {
	if (solid->getType() == GHOST || solid->getType() == SPIDER || solid->getType() == FISH || solid->getType() == SLIME) {

	}
	else if (solid->getType() == COIN) {

	}
	else if (solid->getType() == SPIKES) {

	}
	else if (solid->getType() == TINY_RAY_GUN) {

	}
	else if (solid->getType() == GOLD_KEY) {

	}
	else if (solid->getType() == GOLDKEY_BLOCK) {

	}
	else if (solid->getType() == WATER || solid->getType() == WATERTOP) {

	}
	else if (solid->getType() == BLOCK || solid->getType() == HALF_BLOCK) {
		if (belowCollision(solid)) {
			yVel = 0.0f;
			y += ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.01f);
			onSurface = true;
			if (leftCollision(solid)) {
				xVel = 0.0f;
				x += ((fabs((x - widthFromCenter) - (solid->getXPos() + solid->getWidthFromCenter()))) + 0.01f);
				y -= ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.01f);
			}
			else if (rightCollision(solid)) {
				xVel = 0.0f;
				x -= ((fabs((x + widthFromCenter) - (solid->getXPos() - solid->getWidthFromCenter()))) + 0.01f);
				y -= ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.01f);
			}
			return true;
		}
 		else if (aboveCollision(solid)) { 
			onSurface = false;
			yVel = 0.0f;
			y -= ((fabs((y + heightFromCenter) - (solid->getYPos() - solid->getHeightFromCenter()))) + 0.001f);
			if (leftCollision(solid)) {
				xVel = 0.0f;
				x += ((fabs((x - widthFromCenter) - (solid->getXPos() + solid->getWidthFromCenter()))) + 0.001f);
			}
			else if (rightCollision(solid)) {
				xVel = 0.0f;
				x -= ((fabs((x + widthFromCenter) - (solid->getXPos() - solid->getWidthFromCenter()))) + 0.001f);
			}
			return true;
		}
		else {
			onSurface = false;
			return false;
		}
	}
}	

void Entity::Update(float elapsed)
{
	x += xVel * elapsed;
	y += yVel * elapsed;
	yVel += yAcc * elapsed;
}
void Entity::xTranslate(float shiftX) { x += shiftX; }
void Entity::yTranslate(float shiftY) { y += shiftY; }
void Entity::changeDirection() { dir *= -1; }
void Entity::dies() { alive = false; }
void Entity::Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite) {
	sprite.Draw(program, gameMatrix, x, y);
}