#pragma once
#include "behaviac/agent/agent.h"
#include "behaviac/agent/registermacros.h"
#include "cocos2d.h"
USING_NS_CC;
class PlaneEnemy;
class Enemy :
    public behaviac::Agent
{
public:
    DECLARE_BEHAVIAC_OBJECT(Enemy, behaviac::Agent);

    Enemy();

    ~Enemy();

    void createAnEnemyWithType(int type);

    PlaneEnemy* getPlane();//get plane
    void setPlane(Node* heroPlane);

    void setSpeed(float speed);
    float getSpeed();

    int getEnemyType();
    float getRandomSpeedByInitSpeed(float initSpeed);
    void init();

    bool isAlive();
private:
    Node* nearestEnemy;
    PlaneEnemy* m_plane;

public:
    float speed;
};