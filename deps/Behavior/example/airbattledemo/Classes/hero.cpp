#include "hero.h"
#include "PlaneHero.h"
#include "GameScene.h"
#include "enemy.h"
#include "PlaneEnemy.h"
USING_NS_CC;
/*
void dead(); //飞机挂掉
Enemy* findAnEnemy();//find an enemy plane
bool isWillBeCrashEnemy(Enemy* enemy, Point position);//test whether hero will be crash enemy at position

Enemy* getEnemyInFrontOfPosition(Point position);// get Enemy in front of position
Enemy* getNearestEnemy();//get the nearest enemy
Enemy* getNearestEnemy(Point position);//get the nearest enemy

void AdjustHeroPosition(Hero* hero);
void setHeroPlaneUnderPosition(Point targetPosition);
//m_isAI表示是否使用AI
bool m_isAI;
void setAI(bool value);
//ai动作
void AIAction(float level);
*/
BEGIN_PROPERTIES_DESCRIPTION(Hero)
{
    REGISTER_PROPERTY(m_isAI);

    REGISTER_METHOD(findAnNearestEnemy);
    REGISTER_METHOD(isWillBeCrashOnNearestEnemy);

    REGISTER_METHOD(EscapeEnemyCrash);
    REGISTER_METHOD(hasEnemyInFrontCanAttack);
    REGISTER_METHOD(findAnEnemyAndAttack);

    REGISTER_METHOD(AdjustHeroPosition);
    REGISTER_METHOD(wait20Frame);
    //REGISTER_METHOD(setHeroPlaneUnderPosition).PARAM_DISPLAY_INFO(L"targetPosition");
    
}
END_PROPERTIES_DESCRIPTION()

/******************************************************************
Function    : Hero::Hero()
Date        : 2015-11-18 17:46:09
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : construct function
******************************************************************/
Hero::Hero()
{
    m_plane = PlaneHero::create();
    this->btload("hero");
    this->btsetcurrent("hero");
    waitTime = 20;
    m_isAI = true;
}

Hero::~Hero()
{

}

/******************************************************************
Function    : void findAnNearestEnemy()
Date        : 2015-11-18 10:28:29
Author      : Quinn Pan
Parameter   :
Return      :
Desc        : find an nearest enemy
******************************************************************/
void Hero::findAnNearestEnemy()
{
    m_plane->findAnNearestEnemy();
}

/******************************************************************
Function    : bool Hero::isWillBeCrashOnNearestEnemy()
Date        : 2015-11-18 17:46:54
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : tell whether the hero will be crash on the nearest enemy
******************************************************************/
bool Hero::isWillBeCrashOnNearestEnemy()
{
    return m_plane->isWillBeCrashOnNearestEnemy();
}

/******************************************************************
Function    : void Hero::EscapeEnemyCrash()
Date        : 2015-11-18 17:49:09
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : escape crash on enemy
******************************************************************/
void Hero::EscapeEnemyCrash()
{
    return m_plane->EscapeEnemyCrash();
}


bool Hero::hasEnemyInFrontCanAttack()
{
    return m_plane->hasEnemyInFrontCanAttack();
}

void Hero::findAnEnemyAndAttack()
{
    m_plane->findAnEnemyAndAttack();
}

void Hero::AdjustHeroPosition()
{
    m_plane->AdjustHeroPosition(this->m_plane);
}
bool Hero::wait20Frame(){
    waitTime--;
    if (waitTime == 0)
    {
        waitTime = 20;
        return true;
    }
    return false;

}

/******************************************************************
Function    : void  Hero::setAI(bool value)
Date        : 2015-11-18 17:51:01
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : enable AI 
******************************************************************/
void  Hero::setAI(bool value)
{
    this->m_plane->clearScreenDebugInfo();
    m_isAI = value;
    this->SetActive(value);
    //return m_plane->setAI(value);
}

//AI action
void  Hero::AIAction(float level)
{
    return m_plane->AIAction(level);
}

PlaneHero* Hero::getPlane()
{
    //assert(this->m_plane->getTag() == GameScene::HERO_TAG);
    return m_plane;
}

void Hero::setPlane(PlaneHero* heroPlane)
{
    assert(heroPlane->getTag() == GameScene::HERO_TAG);
    this->m_plane = heroPlane;
}