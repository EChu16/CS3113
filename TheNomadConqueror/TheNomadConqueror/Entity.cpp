#include "Entity.h"

Entity::Entity(float xPos, float yPos, float startingDir, TYPE entityType) : x(xPos), y(yPos), dir(startingDir), type(entityType), pstatic(true), onSurface(true),
alive(true), originalX(xPos), originalY(yPos), originalDir(startingDir), doubleJump(false), changedExp(false),randNum(0) {
	if (entityType == PLAYER) {
		hp = 3.0f;
		originalHp = 3.0f;
		xVel = 0.0f;
		origXVel = 0.0f;
		yVel = 0.0f;
		origYVel = 0.0f;
		xAcc = 0;
		origXAcc = 0.0;
		yAcc = -20.0f;
		origYAcc = -20.0f;
		coins = 0;  
		widthFromCenter = 0.47f;
		heightFromCenter = 1.00f;
		goldKey = false;
		sinceLastBullet = 0.6f;
		stallShootBullet = 0.6f;
		sinceChangeExpTime = 0.0f;
	}
	else if (entityType == TINY_RAY_GUN) {
		xVel = 0.0f;
		yVel = 0.0f;
		xAcc = 0;
		yAcc = -20.0f;
	}
	else if (entityType == BLOCK) {
		widthFromCenter = 0.5f;
		heightFromCenter = 0.5f;
	}
	else if (entityType == HALF_BLOCK) {
		widthFromCenter = 0.5f;
		heightFromCenter = 0.125f;
	}
	else if (entityType == REG_BULLET || entityType == BIG_BULLET) {
		expireTime = 0.8f;
		timeAlive = 0.0f;
		xVel = 8.0f;
		yVel = 3.0f;
		widthFromCenter = 0.4f;
		heightFromCenter = 0.125f;
	}
	else if (entityType == SPIDER) {
		hp = 3.0f;
		originalHp = 3.0f;
		xVel = 0.0f;
		yVel = 0.0f;
		xAcc = 0;
		yAcc = -25.0f;
		origXVel = 0.0f;
		origYVel = 0.0f;
		origXAcc = 0.0f;
		origYAcc = -25.0f;
		widthFromCenter = 0.45f;
		heightFromCenter = 0.3f;
		onSurface = false;
	}
	else if (entityType == GHOST) {
		hp = 4.0f;
		originalHp = 4.0f;
		xVel = 1.5f;
		yVel = 1.5f;
		xAcc = 0.0f;
		yAcc = 0.0f;
		origXVel = 1.5f;
		origYVel = 1.5f;
		origXAcc = 0.0f;
		origYAcc = 0.0f;
		widthFromCenter = 0.30f;
		heightFromCenter = 0.4f;
		perimeter = 5.0f;
		ghostState = PASSIVE;
	}
	else if (entityType == FISH) {
		hp = 1.0f;
		originalHp = 1.0f;
		xVel = 0.75f;
		yVel = 0.0f;
		xAcc = 0.0f;
		yAcc = 0.0f;
		origXVel = 0.75f;
		origYVel = 0.0f;
		origXAcc = 0.0f;
		origYAcc = 0.0f;
		widthFromCenter = 0.8f;
		heightFromCenter = 0.2f;
	}
}
float Entity::getHp() const { return hp; }
float Entity::getXPos() const{ return x; }
float Entity::getYPos() const{ return y; }
float Entity::getXVel() const { return xVel; }
float Entity::getYVel() const { return yVel; }
float Entity::getXAcc() const { return xAcc; }
float Entity::getYAcc() const { return yAcc; }
float Entity::getDirection() const { return dir; }
float Entity::getWidthFromCenter() const{ return widthFromCenter; }
float Entity::getHeightFromCenter() const{ return heightFromCenter; }
float Entity::getTimeAlive() const { return timeAlive; }
float Entity::getExpireTime() const { return expireTime; }
float Entity::getSinceLastExpTime() const { return sinceChangeExpTime; }
float Entity::getStallShootTime() const { return stallShootBullet; }
float Entity::getLastBulletTime() const { return sinceLastBullet; }
bool Entity::canDoubleJump() const { return doubleJump; }
bool Entity::isStatic() const { return pstatic; }
bool Entity::isOnSurface() const { return onSurface; }
bool Entity::isAlive() const { return alive; }
bool Entity::normExp() const { return !changedExp; }
TYPE Entity::getType() const{ return type; }
SheetSprite& Entity::getMainSprite() { return staticSprite; }
SheetSprite& Entity::getCurrentSprite() { return currentSprite; }
bool Entity::hasGoldKey() const { return goldKey; }
bool Entity::hasTinyGun() const {
	for (int i = 0; i < items.size(); i++) {
		if (items[i]->getType() == TINY_RAY_GUN) {
			return true;
		}
	}
	return false;
}
bool Entity::rightCollision(Entity* solid) const {
	return (this->getXPos() + this->getWidthFromCenter() > solid->getXPos() - solid->getWidthFromCenter())
		&& (this->getYPos() + this->getHeightFromCenter() > solid->getYPos() - solid->getHeightFromCenter())
		&& (this->getYPos() - this->getHeightFromCenter() < solid->getYPos() + solid->getHeightFromCenter())
		&& (this->dir == -1.0f);
}
bool Entity::leftCollision(Entity* solid) const {
	return (this->getXPos() - this->getWidthFromCenter() < solid->getXPos() + solid->getWidthFromCenter())
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
	if ((solid->getType() == GHOST || solid->getType() == SPIDER || solid->getType() == FISH) && type == PLAYER && solid->isAlive()) {
		if (this->getXPos() - this->getWidthFromCenter() <= solid->getXPos() + solid->getWidthFromCenter() && dir == -1.0f) {
			for (int i = 0; i < items.size(); i++) {
				if (items[i]->getType() == TINY_RAY_GUN) {
					items[i]->xTranslate(((fabs((x - widthFromCenter) - (solid->getXPos() + solid->getWidthFromCenter()))) + 0.4f));
					items[i]->changeXVel(0.0f);
					items[i]->yTranslate(-1 * ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.4f));
					break;
				}
			}
			xVel = 0.0f;
			x += ((fabs((x - widthFromCenter) - (solid->getXPos() + solid->getWidthFromCenter()))) + 0.15f);
			y -= ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.15f);	
		}
		else if (this->getXPos() + this->getWidthFromCenter() >= solid->getXPos() - solid->getWidthFromCenter() && dir == 1.0f) {
			doubleJump = false;
			for (int i = 0; i < items.size(); i++) {
				if (items[i]->getType() == TINY_RAY_GUN) {
					items[i]->xTranslate(-1 * ((fabs((x + widthFromCenter) - (solid->getXPos() - solid->getWidthFromCenter()))) + 0.4f));
					items[i]->changeXVel(0.0f);
					items[i]->yTranslate(-1 * ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.4f));
					break;
				}
			}
			xVel = 0.0f;
			x -= ((fabs((x + widthFromCenter) - (solid->getXPos() - solid->getWidthFromCenter()))) + 0.15f);
			y -= ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.15f);
		}
		hp -= 0.5f;
		return true;
	}
	else if ((type == REG_BULLET || type == BIG_BULLET) && (solid->isAlive()) && (solid->getType() == SPIDER || solid->getType() == GHOST || solid->getType() == FISH)) {
		if (solid->decreaseHP(1.0f)) {
			solid->dies();
		}
		alive = false;
		return true;
	}
	else if (solid->getType() == ENEMY_BULLET && type == PLAYER && solid->isAlive()) {
		hp -= 0.5f;
		return true;
	}
	else if (solid->getType() == COIN && type == PLAYER) {
		solid->dies();
		this->coins++;
		return true;
	}
	else if (solid->getType() == SPIKE && type == PLAYER) {
		if (this->getXPos() - this->getWidthFromCenter() <= solid->getXPos() + solid->getWidthFromCenter() && dir == -1.0f) {
			for (int i = 0; i < items.size(); i++) {
				if (items[i]->getType() == TINY_RAY_GUN) {
					items[i]->xTranslate(((fabs((x - widthFromCenter) - (solid->getXPos() + solid->getWidthFromCenter()))) + 0.6f));
					items[i]->changeXVel(0.0f);
					items[i]->yTranslate(-1 * ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.6f));
					break;
				}
			}
			xVel = 0.0f;
			x += ((fabs((x - widthFromCenter) - (solid->getXPos() + solid->getWidthFromCenter()))) + 0.6f);
			y -= ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.6f);
		}
		else if (this->getXPos() + this->getWidthFromCenter() >= solid->getXPos() - solid->getWidthFromCenter() && dir == 1.0f) {
			doubleJump = false;
			for (int i = 0; i < items.size(); i++) {
				if (items[i]->getType() == TINY_RAY_GUN) {
					items[i]->xTranslate(-1 * ((fabs((x + widthFromCenter) - (solid->getXPos() - solid->getWidthFromCenter()))) + 0.6f));
					items[i]->changeXVel(0.0f);
					items[i]->yTranslate(-1 * ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.6f));
					break;
				}
			}
			xVel = 0.0f;
			x -= ((fabs((x + widthFromCenter) - (solid->getXPos() - solid->getWidthFromCenter()))) + 0.6f);
			y -= ((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.6f);
		}
		hp -= 0.5f;
		return true;
	}
	else if (solid->getType() == TINY_RAY_GUN && type == PLAYER) {
		if (solid->isAlive() && !hasTinyGun()) {
			solid->setImmX(x + (dir*0.2f));
			solid->dies();
			this->items.push_back(solid);
			return true;
		}
	}
	else if (solid->getType() == GOLD_KEY && type == PLAYER) {
		solid->dies();
		goldKey = true;
		return true;
	}
	else if (solid->getType() == WATER && type == PLAYER) {
		doubleJump = true;
		return true;
	}
	else if (solid->getType() == BLOCK || solid->getType() == HALF_BLOCK) {
		if (type == FISH) {
			dir *= -1;
			return true;
		}
		else if (belowCollision(solid) && (type == PLAYER || type == SPIDER)) {
			if (hasTinyGun()) {
				for (int i = 0; i < items.size(); i++) {
					if (items[i]->getType() == TINY_RAY_GUN) {
						items[i]->yTranslate(((fabs((y - heightFromCenter) - (solid->getYPos() + solid->getHeightFromCenter()))) + 0.01f));
						items[i]->changeYVel(0.0f);
					}
				}
			}
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
		else if (aboveCollision(solid) && type == PLAYER) {
			if (hasTinyGun()) {
				for (int i = 0; i < items.size(); i++) {
					if (items[i]->getType() == TINY_RAY_GUN) {
						items[i]->yTranslate(-((fabs((y + heightFromCenter) - (solid->getYPos() - solid->getHeightFromCenter()))) + 0.001f));
						items[i]->changeYVel(0.0f);
					}
				}
			}
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
	doubleJump = false;
	return false;
}
void Entity::isNear(Entity* solid) {
	if (sqrt((pow((x - solid->getXPos()), 2)) + (pow((y - solid->getYPos()), 2))) <= perimeter || ghostState == CHASE) {
		ghostState = CHASE;
		if (hp <= 3.0) {
			xVel = 1.5f;
		}
		if (x > solid->getXPos()) {
			xVel = -1 * fabs(dir);
			dir = -1;
		}
		else if (x < solid->getXPos()) {
			xVel = fabs(dir);
			dir = 1;
		}
		if (y > solid->getYPos()) {
			yVel = -1 * fabs(yVel);
		}
		else if(y < solid->getYPos()){
			yVel = fabs(yVel);
		}
	}
}

void Entity::setMainSprite(SheetSprite& mainSprite) { mainSprite = mainSprite; staticSprite = mainSprite; }
void Entity::setCurrentSprite(SheetSprite& currSprite, float theExpTime) { 
	currentSprite = currSprite; 
	if (currentSprite != staticSprite) {
		changedExp = true;
		expTime = theExpTime;
		sinceChangeExpTime = 0;
	}
}
bool Entity::decreaseHP(float damageTaken) {
	hp -= damageTaken; 
	if (hp == 0) { return true; }
	return false;
}
void Entity::setImmX(float immX) { x = immX; }
void Entity::setImmY(float immY) { y = immY; }
void Entity::respawn() {
	x = originalX; y = originalY; dir = originalDir; currentSprite = staticSprite; alive = true; goldKey = false;
	for (int i = 0; i < items.size(); i++) {
		if (items[i]->getType() == TINY_RAY_GUN && type == PLAYER) {
			items[i] = items[items.size() - 1];
			items.pop_back();
		}
	}
	if (type == TINY_RAY_GUN) {
		xVel = 0.0f;
		yVel = 0.0f;
		xAcc = 0;
		yAcc = -20.0f;
	}
}
void Entity::resetOriginalValues() {
	if (type == PLAYER || type == SPIDER || type == FISH || type == GHOST) {
		hp = originalHp;
		xVel = origXVel;
		yVel = origYVel;
		xAcc = origXAcc;
		yAcc = origYAcc;
		coins = 0;
		if (type == SPIDER) {
			onSurface = false;
		}
		if (type == GHOST) {
			ghostState = PASSIVE;
		}
	}
}
void Entity::changeStatic(bool newStatic) { pstatic = newStatic; }
void Entity::changeOnSurface(bool newOnSurface) { onSurface = newOnSurface; }
void Entity::resetLastBulletTime() { sinceLastBullet = 0; }
void Entity::xTranslate(float shiftX) { x += shiftX; }
void Entity::yTranslate(float shiftY) { y += shiftY; }
void Entity::changeXVel(float newX) { xVel = newX; }
void Entity::changeYVel(float newY) { yVel = newY; }
void Entity::changeDirection(float newDir) { dir = newDir; }
void Entity::revive() { alive = true; }
void Entity::dies() { alive = false; }
void Entity::animate(float elapsed) {	
	if (type == SPIDER) {
		if (onSurface == true) {
			randNum = rand() % 5 + 7;
			yVel = float(randNum);
			dir *= -1;
			onSurface = false;
		}
		Update(elapsed);
	}
	else if (type == GHOST) {
		if (ghostState == CHASE) {
			x += xVel * elapsed;
			y += yVel * elapsed;
		}
	}
	else if (type == FISH) {
		x += xVel * elapsed * dir;
	}
}
void Entity::Update(float elapsed)
{
	if (type == REG_BULLET || type == BIG_BULLET || type == ENEMY_BULLET) {
		if (dir == 1.0f) { xVel = fabs(xVel); }
		else { xVel = fabs(xVel) * -1.0f; }
		y -= yVel* elapsed;
		if (timeAlive >= expireTime) { alive = false; }
		else { timeAlive += elapsed; }
	}
	if (hasTinyGun() && type == PLAYER) {
		sinceLastBullet += elapsed;
	}
	if (type == PLAYER) {
		if (hp == 0) {
			alive = false;
		}
	}
	if (type == PLAYER && changedExp) {
		sinceChangeExpTime += elapsed;
		if (sinceChangeExpTime >= expTime) { changedExp = false; }
	}
	x += xVel * elapsed;
	y += yVel * elapsed;
	yVel += yAcc * elapsed;
}
void Entity::Draw(ShaderProgram* program, Matrix& gameMatrix) {
	if (type == FISH || type == SPIDER || type == GHOST) {
		currentSprite.Draw(program, gameMatrix, x, y, -1 * dir);
	}
	else {
		currentSprite.Draw(program, gameMatrix, x, y, dir);
	}
}