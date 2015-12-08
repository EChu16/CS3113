#include "Entity.h"

Entity::Entity(float xPos, float yPos, float startingDir, TYPE entityType, SheetSprite& mainSprite) : x(xPos), y(yPos), dir(startingDir), type(entityType), pstatic(true), onSurface(true), alive(true){
	staticSprite = mainSprite;
	currentSprite = mainSprite;
	if (entityType == PLAYER) {
		xVel = 0.0f;
		yVel = 0.0f;
		xAcc = 0;
		yAcc = -12.0f;
		widthFromCenter = 0.335f;
		heightFromCenter = 0.46f;
	}
	else if (entityType == T_BLOCK || entityType == L_BLOCK || entityType == R_BLOCK || entityType == TR_BLOCK || entityType == TL_BLOCK ||
		entityType == ITEM_BLOCK || entityType == TL_HALF_BLOCK || entityType == T_HALF_BLOCK || entityType == TR_HALF_BLOCK || entityType == TL_TRI_BLOCK
		|| entityType == TR_TRI_BLOCK || entityType == TL_CURVE_BLOCK || entityType == TR_CURVE_BLOCK || entityType == CRATE || entityType == GOLDKEY_BLOCK
		|| entityType == WATER || entityType == WATERTOP) {
		x += 0.479f;
		y += 0.5145f;
		//widthFromCenter = 0.479f;
		//heightFromCenter = 0.5145f;
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
void Entity::changeStatic(bool newStatic) { pstatic = newStatic; }
void Entity::changeOnSurface(bool newOnSurface) { onSurface = newOnSurface; }
void Entity::changeXVel(float newX) { xVel = newX; }
void Entity::changeYVel(float newY) { yVel = newY; }
SheetSprite& Entity::getMainSprite() { return staticSprite; }
bool Entity::checkAlive() const{ return alive; }
TYPE Entity::getType() const{ return type; }
float Entity::getWidthFromCenter() const{ return widthFromCenter; }
float Entity::getHeightFromCenter() const{ return heightFromCenter; }

bool Entity::collidesWith(Entity* solid) const {
	if ((this->getXPos() + this->getWidthFromCenter() > solid->getXPos() - solid->getWidthFromCenter()) || (this->getXPos() - this->getWidthFromCenter() < solid->getXPos() + solid->getWidthFromCenter())
		&& this->getYPos() + this->getHeightFromCenter() ) {
		
	}
	else return false;
}

void Entity::collidedAction(Entity* solid) {
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
	else {

	}
}

void Entity::Update(float elapsed)
{
	x += xVel * elapsed;
	y += yVel * elapsed;
	if (!isOnSurface()) {
		yVel += yAcc * elapsed;
	}
	
}
void Entity::xTranslate(float shiftX) { x += shiftX; }
void Entity::yTranslate(float shiftY) { y += shiftY; }
void Entity::changeDirection() { dir *= -1; }
void Entity::dies() { alive = false; }
void Entity::Draw(ShaderProgram* program, Matrix& gameMatrix, SheetSprite& sprite) {
	sprite.Draw(program, gameMatrix, x, y);
}