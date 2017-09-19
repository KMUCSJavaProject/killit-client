//
//  Player.cpp
//  Killit
//
//  Created by a on 2017. 9. 4..
//
//

#include "Player.h"

#include "HelloWorldScene.h"
#include "ZOrder.h"
#include "CameraUtil.h"

USING_NS_CC;

Player *Player::create(int sx, int sy) {
    Player *ret = new (std::nothrow) Player();
    if (ret && ret->init())
    {
        ret->autorelease();
		ret->gX = sx;
		ret->gY = sy;
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool Player::init() {
    setScale(2);
    
    player = Sprite::create("res/player.png");
    player->getTexture()->setAliasTexParameters();
    this->addChild(player);

	/// «√∑π¿ÃæÓ zorder º≥¡§
	this->setGlobalZOrder(ZORDER::PLAYER);
	for (auto &i : this->getChildren()) {
		i->setGlobalZOrder(ZORDER::PLAYER);
	}
    
    scheduleUpdate();
    
    return true;
}

void Player::update(float dt) {
	float pAngle = CC_RADIANS_TO_DEGREES(angle.getAngle());
	if (pAngle > 90 || pAngle <= -90) { /// øﬁ¬ 
		player->setFlippedX(true);
	} else {
		player->setFlippedX(false);
	}
}

void Player::onStickBegan(Vec2 angle, Ref *pSender) {
    this->angle = angle;
    touchJoystick = true;
}

void Player::onStickMoved(Vec2 angle, Ref *pSender) {
    this->angle = angle;
}

void Player::onStickEnded(Vec2 angle, Ref *pSender) {
    touchJoystick = false;
}

void Player::calculateGridCoord(int mapWidth, int mapHeight) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
	gX = (this->getPositionX() + (24 * (mapWidth - 1) - origin.x)) / 48 + 1;
	gY = (this->getPositionY() + (24 * (mapHeight - 1) - origin.y)) / 48 + 1;
}

void Player::gridCoordUpdate(int mapWidth, int mapHeight) {
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;
    tempPosition = Vec2(48 * gX - (24 * mapWidth - origin.x), 48 * gY - (24 * mapHeight - origin.y));
	this->setPosition(tempPosition);
}

void Player::move() {
	/// «√∑π¿ÃæÓ∞° ¡∂¿ÃΩ∫∆Ω ≈Õƒ°«ﬂ¿ª ∂ß «ÿ¥Á πÊ«‚¿∏∑Œ ¿Ãµø
	if (touchJoystick) {
		tempPosition = getPosition() + angle * speed;
	}
}

void Player::collision() {
	HelloWorld *parent = (HelloWorld *)getParent();
    
    
    
	/// «ˆ¿Á ¿ßƒ°∏¶ ±‚¡ÿ¿∏∑Œ 3x3ƒ≠¿ª √Êµπ ∞ÀªÁ∏¶ «‘
	for (int i = MAX(gY - 1, 0); i < MIN(gY + 2, parent->mapHeight); i++) {
		for (int j = MAX(gX - 1, 0); j < MIN(gX + 2, parent->mapWidth); j++) {
            if (i == gY && j == gX) continue;
            /// ∫Æ¿Ã≥™ πÆ¿Ã∏È
            if (parent->checkSolidObject(j, i)) {
                /// «√∑π¿ÃæÓøÕ ªÛ¥Î √Êµπ√º¿« BoundingBox∏¶ ∞°¡Æø»
                auto playerBB = Rect(tempPosition - player->getContentSize(), player->getContentSize() * 2);
                auto otherBB = Rect(parent->mapTile[i][j]->getPosition() - Size(24, 24), Size(48, 48));
				/// √Êµπ ∞ÀªÁ
				if (playerBB.intersectsRect(otherBB)) {
					parent->mapTile[i][j]->setColor(Color3B::RED);

					/// 0: left, 1: right, 2: up, 3: down
					int dir = 0;

					float angle = CC_RADIANS_TO_DEGREES((Vec2(playerBB.getMidX(), playerBB.getMidY()) - Vec2(otherBB.getMidX(), otherBB.getMidY())).getAngle());

					if (angle > -45 && angle <= 45) dir = 1;
					else if (angle > 45 && angle <= 135) dir = 2;
					else if ((angle > 135 && angle <= 180) || (angle < -135 && angle >= -180)) dir = 0;
					else dir = 3;
                    
                    if (dir == 0)
                        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString("left");
                    if (dir == 1)
                        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString("right");
                    if (dir == 2)
                        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString("up");
                    if (dir == 3)
                        CameraUtil::getInstance()->fixedLayer->getChildByName<Label*>("debug1")->setString("down");
                    
//                    parent->getChildByName<DrawNode*>("debug2")->drawLine(playerBB.origin, otherBB.origin, Color4F::RED);

					if (dir == 0) {
						tempPosition.x = otherBB.getMinX() - player->getContentSize().width;
					} else if (dir == 1) {
						tempPosition.x = otherBB.getMaxX() + player->getContentSize().width;
					} else if (dir == 2) {
						tempPosition.y = otherBB.getMaxY() + player->getContentSize().height;
					} else {
						tempPosition.y = otherBB.getMinY() - player->getContentSize().height;
					}
				}
			}

		}
	}
}

void Player::updateZOrder() {
	HelloWorld *parent = (HelloWorld *)getParent();

	/// «√∑π¿ÃæÓ z-order ∫Ø∞Ê
	int zorder = parent->mapTile[int(clampf(gY, 0, parent->mapHeight - 1))][int(clampf(gX, 0, parent->mapWidth - 1))]->getGlobalZOrder() + 1000;
	this->setGlobalZOrder(zorder);
	for (auto &i : this->getChildren()) {
		i->setGlobalZOrder(zorder);
	}
}

void Player::updatePosition() {
	/// ∞ËªÍµ» «√∑π¿ÃæÓ ¿ßƒ° Ω«¡¶ ¿˚øÎ
	this->setPosition(tempPosition);
	CameraUtil::getInstance()->setPosition(this->getPosition());
}

void Player::checkGameObjects() {
	HelloWorld *parent = (HelloWorld *)getParent();
	Vec2 origin = Director::getInstance()->getVisibleSize() / 2;

	Vec2 check = this->getPosition() + angle * 48;

    int xx = (check.x + (24 * (parent->mapWidth - 1) - origin.x)) / 48 + 1;
	int yy = (check.y + (24 * (parent->mapHeight - 1) - origin.y)) / 48 + 1;

//	parent->mapTile[yy][xx]->setColor(Color3B::BLUE);

	//state = 0;
	switch (parent->mapData[yy][xx]) {
	case 1:
		parent->mapTile[yy][xx]->setColor(Color3B::GREEN);
		//state = 1;
		break;
	case 2:
		parent->mapTile[yy][xx]->setColor(Color3B::YELLOW);
		//state = 2;
		break;
	}
}

