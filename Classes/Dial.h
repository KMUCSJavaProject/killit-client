//
//  Dial.h
//  Killit
//
//  Created by a on 2017. 10. 27..
//

#ifndef Dial_h
#define Dial_h

#include "cocos2d.h"

class Player;

// 플레이어의 공격 방향을 조절하는 ui클래스.
// 임시로 오브젝트 상호작용 기능도 추가함.
class Dial : public cocos2d::Node {
public:
    static Dial *create();
    
    bool init() override;
    void bind(Player *player);
    
public:
    void onTouchBegan(const cocos2d::Vec2 &, int id);
    void onTouchMoved(const cocos2d::Vec2 &, int id);
    void onTouchEnded(const cocos2d::Vec2 &, int id);
    
public:
    float prevAngle = 0, sensitivity = 2.5f;
    /// 변수 이름 바꿔야 함..
    Player *bnd = nullptr;
    int id = -1;
    
    bool findObject = false;
    
};

#endif /* Dial_h */