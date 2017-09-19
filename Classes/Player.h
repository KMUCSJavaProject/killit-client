//
//  Player.h
//  Killit
//
//  Created by a on 2017. 9. 4..
//
//

#ifndef Player_h
#define Player_h

#include "cocos2d.h"

class Player : public cocos2d::Node {
public:
	/// ���� x��ǥ, ���� y��ǥ
    static Player *create(int sx, int sy);
    
    bool init() override;
    void update(float dt) override;
    
    void onStickBegan(cocos2d::Vec2 angle, cocos2d::Ref *pSender);
    void onStickMoved(cocos2d::Vec2 angle, cocos2d::Ref *pSender);
    void onStickEnded(cocos2d::Vec2 angle, cocos2d::Ref *pSender);

	/// �̸� ���� �ʿ�
	/// ���� ��ǥ�� �׸��� ��ǥ�� ��ȯ�ϴ� �Լ�
	void calculateGridCoord(int mapWidth, int mapHeight);
	/// �׸��� ��ǥ�� ���� ��ǥ�� �ݿ���Ű�� �Լ�
	void gridCoordUpdate(int mapWidth, int mapHeight);

	/// ��� ������ ��Ȯ�� �ؾ��ؼ� ���� ���� �Լ���
	void move();
	void collision();
	void updateZOrder();
	void updatePosition();
	void checkGameObjects();
    
public:
    cocos2d::Sprite *player;
	cocos2d::Size playerSize;
	/// angle: �÷��̾ ���� �ִ� ����, pos: �÷��̾� �ӽ� ��ǥ
    cocos2d::Vec2 angle, tempPosition;

    bool touchJoystick = false;
    float speed = 4.5f;
	/// Ÿ�ϸʿ����� ��ǥ
	int gX = 0, gY = 0;
    
};

#endif /* Player_h */
