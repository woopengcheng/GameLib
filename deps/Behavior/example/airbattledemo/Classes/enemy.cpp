
#include "enemy.h"
#include "PlaneEnemy.h"
#include "GameScene.h"
USING_NS_CC;

BEGIN_PROPERTIES_DESCRIPTION(Enemy)
{
    //REGISTER_PROPERTY(m_isAI);

    REGISTER_METHOD(getSpeed);
    REGISTER_METHOD(getEnemyType);
    REGISTER_METHOD(isAlive);
    REGISTER_METHOD(setSpeed).PARAM_DISPLAY_INFO(L"speed");
    REGISTER_METHOD(getRandomSpeedByInitSpeed).PARAM_DISPLAY_INFO(L"iniSpeed");
    //REGISTER_METHOD(AdjustHeroPosition).PARAM_DISPLAY_INFO(L"hero");
    //REGISTER_METHOD(setHeroPlaneUnderPosition).PARAM_DISPLAY_INFO(L"_targetPosition");

}
END_PROPERTIES_DESCRIPTION()

/******************************************************************
Function    : Enemy::Enemy()
Date        : 2015-11-18 17:45:24
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : construct function
******************************************************************/
Enemy::Enemy()
{
    this->btload("enemy_fsm");
    this->btsetcurrent("enemy_fsm");
    //this->btload("enemy");
    //this->btsetcurrent("enemy");
    speed = 20.0f;
}

Enemy::~Enemy()
{

}

//create an enemy with type
void Enemy::createAnEnemyWithType(int type)
{
    m_plane = PlaneEnemy::createWithEnemyType(type);
    m_plane->myAgent = this;
}

/******************************************************************
Function    : PlaneEnemy* Enemy::getPlane()
Date        : 2015-11-18 17:41:41
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : get agent's plane
******************************************************************/
PlaneEnemy* Enemy::getPlane()
{
    return m_plane;
}


/******************************************************************
Function    : void Enemy::setPlane(Node* plane)
Date        : 2015-11-18 17:42:12
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : set the agent's plane
******************************************************************/
void Enemy::setPlane(Node* plane)
{
    this->m_plane = (PlaneEnemy*)plane;
}


/******************************************************************
Function    : void Enemy::setSpeed(float speed)
Date        : 2015-11-13 10:41:44
Author      : Quinn Pan
Parameter   :
Return      :
Desc        : set the plane's speed
******************************************************************/
void Enemy::setSpeed(float speed)
{
    this->speed = speed;//fix crash bug
    
    //if (this->m_plane)
    //{
    //    this->m_plane->setSpeed(speed);//this code will cause crash. but if use the code before this line will  be not
    //}
    //this->m_plane->setSpeed(speed);
}



/******************************************************************
Function    : float getSpeed::Enemy()
Date        : 2015-11-13 10:41:23
Author      : Quinn Pan
Parameter   :
Return      :
Desc        : Get the enemy's speed
******************************************************************/
float Enemy::getSpeed()
{
    if (this->m_plane){
        return this->m_plane->getSpeed();
    }
    else{
        return 0.0f;
    }
   
}

/******************************************************************
Function    : int Enemy::getEnemyType()
Date        : 2015-11-13 10:27:26
Author      : Quinn Pan
Parameter   : NULL
Return      : NULL
Desc        : get the enemy type
******************************************************************/
int Enemy::getEnemyType()
{
    return this->m_plane->getType();
}


/******************************************************************
Function    : float Enemy::getRandomSpeedByInitSpeed(float initSpeed)
Date        : 2015-11-13 11:47:32
Author      : Quinn Pan
Parameter   : float initSpeed
Return      : random speed
Desc        : random value large than half of initSpeed and below 1.2 times of initSpeed
******************************************************************/
float Enemy::getRandomSpeedByInitSpeed(float initSpeed)
{
    return  random(0.5, 1.3)*initSpeed;
}

/******************************************************************
Function    : void Enemy::init()
Date        : 2015-11-13 10:42:58
Author      : Quinn Pan
Parameter   :
Return      :
Desc        : init the behaviac's config
******************************************************************/
void Enemy::init()
{

}

/******************************************************************
Function    : bool Enemy::isAlive()
Date        : 2015-11-13 11:59:26
Author      : Quinn Pan
Parameter   :
Return      : bool
Desc        : tell the enemy is live or not
******************************************************************/
bool Enemy::isAlive()
{
    if (this->m_plane)
    {
        return true;
    }
    else
    {
        
        //if the enemy is dead, then add this agent's behaviac to GanmeScene's agent delete queue.
        GameScene::addBehaviacAgentDeleteQueue((behaviac::Agent*)this);
        return false;
    }
}
