#include "NPC.h"
#include "PlaneEnemy.h"
#include "GameScene.h"
#include "enemy.h"

BEGIN_PROPERTIES_DESCRIPTION(NPC)
{
    REGISTER_PROPERTY(Level1Up_Score);
    REGISTER_PROPERTY(Level2Up_Score);
    REGISTER_PROPERTY(Level3Up_Score);
    REGISTER_PROPERTY(Level4Up_Score);
    REGISTER_PROPERTY(m_canBossRefresh);
    REGISTER_PROPERTY(m_level);

    REGISTER_METHOD(getLevel).PARAM_DISPLAY_INFO(L"score");
    REGISTER_METHOD(setLeveUpScore).PARAM_DISPLAY_INFO(L"level").PARAM_DISPLAY_INFO(L"score");
    REGISTER_METHOD(spawnEnemy);
}
END_PROPERTIES_DESCRIPTION()


//BEGIN_PROPERTIES_DESCRIPTION(NPC::refresh_delay_type)
//{
//    //CLASS_DISPLAYNAME(L"测试Struct")
//    //CLASS_DESC(L"测试Struct的说明")
//
//    REGISTER_PROPERTY(level0_daly);
//    REGISTER_PROPERTY(level1_daly);
//    REGISTER_PROPERTY(level2_daly);
//    REGISTER_PROPERTY(level3_daly);
//    REGISTER_PROPERTY(level4_daly);
//}
//END_PROPERTIES_DESCRIPTION()


NPC::NPC()
{
    this->initData();
}


NPC::~NPC()
{
}


/******************************************************************
Function    : void NPC::spawnEnemy()
Date        : 2015-11-13 14:13:03
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : Spawn an enemy
******************************************************************/
void NPC::spawnEnemy()
{
    //log("refresh an enemy");

    auto winSize = Director::getInstance()->getWinSize();

    int enemy_type = PlaneEnemy::Enemy1;

    //根据游戏Level选择性随机刷新不同种类的战机
    switch (m_level)
    {
    case GameScene::LEVEL1:
        break;
    case GameScene::LEVEL2:
        enemy_type = random(0, 1);
        break;
    case GameScene::LEVEL3:
        enemy_type = random(0, 2);
        if (m_canBossRefresh && enemy_type == 2) //如果刷新出boss战机，那么禁止其继续刷新，直到该禁止标志被重置
            m_canBossRefresh = false;
        else enemy_type = random(0, 1);
        break;
    case GameScene::LEVEL4:
        enemy_type = random(0, 3);
        if (m_canBossRefresh && enemy_type >= 2)
            m_canBossRefresh = false;
        else enemy_type = random(0, 1);
        break;
    case GameScene::LEVEL5:
        enemy_type = random(0, 3);
        if (m_canBossRefresh && enemy_type >= 2)
            m_canBossRefresh = false;
        else enemy_type = random(0, 1);
        break;
    }

    auto enemy = behaviac::Agent::Create<Enemy>();
    enemy->createAnEnemyWithType(enemy_type);
    ////根据战机类型加入战机
    PlaneEnemy* enemy_plane = enemy->getPlane();
    ((Node*)this->getParent())->addChild(enemy_plane, 0, GameScene::ENEMY_TAG);
    //behaviac::Agent::Destroy(enemy);

    //enemy->SetActive(false);
    //enemy_plane1->setColor(Color3B(255, 0, 255));

    //auto enemy_plane = PlaneEnemy::createWithEnemyType(enemy_type);
    //((Node*)this->getParent())->addChild(enemy_plane, 0, GameScene::ENEMY_TAG);

    //设定战机初始位置的X轴的取值范围，根据这个范围随机设置战机初始X轴位置
    int min = enemy_plane->getContentSize().width / 2;
    int max = winSize.width - enemy_plane->getContentSize().width / 2;
    enemy_plane->setPosition(Vec2(random(min, max), winSize.height + enemy_plane->getContentSize().height / 2));

    //给敌机一个body
    Vec2 vec[10]; //存放敌方战机的多边形点
    memset(vec, 0, sizeof(vec));
    int vec_count = 0;
    switch (enemy_type)
    {
    case PlaneEnemy::Enemy1:
        vec[0] = Vec2(0, -18);
        vec[1] = Vec2(-24, 6);
        vec[2] = Vec2(24, 6);
        vec_count = 3;
        break;
    case PlaneEnemy::Enemy2:
        vec[0] = Vec2(0, -40);
        vec[1] = Vec2(-40, 0);
        vec[2] = Vec2(0, 45);
        vec[3] = Vec2(40, 0);
        vec_count = 4;
        break;
    default:
        vec[0] = Vec2(35, -120);
        vec[1] = Vec2(-35, -120);
        vec[2] = Vec2(-80, -88);
        vec[3] = Vec2(-80, 130);
        vec[4] = Vec2(80, 130);
        vec[5] = Vec2(80, -88);
        vec_count = 6;
        break;
    }
    //auto enemybody = PhysicsBody::createBox(enemy_plane->getContentSize());
    auto enemybody = PhysicsBody::create();
    enemybody->addShape(PhysicsShapePolygon::create(vec, vec_count));
    enemybody->setCollisionBitmask(0x0); //不进行碰撞模拟，因为不需要
    enemybody->setContactTestBitmask(GameScene::ENEMY_CONTACTMASKBIT);
    enemy_plane->setPhysicsBody(enemybody);
}

void* NPC::getParent()
{
    return m_parent;
}

// set NPC parent
void NPC::setParent(const void* parent)
{
    this->m_parent = const_cast<void *>(parent);
}

void NPC::setLeveUpScore(int level, int score)
{
    switch (level)
    {
    case 1:
        Level1Up_Score = score;
        break;
    case 2:
        Level2Up_Score = score;
        break;
    case 3:
        Level3Up_Score = score;
        break;
    case 4:
        Level4Up_Score = score;
        break;
    default:
        break;
    }
}

int NPC::getGameLevel()
{
    return m_level;
}


NPC::NPC(void* parent)
{
    this->m_parent = parent;
    this->initData();
}

void NPC::initData()
{
    Level1Up_Score = 0;
    Level2Up_Score = 200;
    Level3Up_Score = 500;
    Level4Up_Score = 1000;

    level0_daly = 20;
    level1_daly = 15;
    level2_daly = 10;
    level3_daly = 5;
    level4_daly = 2;
    m_level = 0;
}

// set the game current level
int NPC::getLevel(int m_score)
{
    m_level=GameScene::LEVEL1;
    //根据到达的分数，设置LEVEL
    if (m_score >= Level1Up_Score && m_score < Level2Up_Score)
        m_level = GameScene::LEVEL2;
    else if (m_score >= Level2Up_Score && m_score < Level3Up_Score)
        m_level = GameScene::LEVEL3;
    else if (m_score >= Level3Up_Score && m_score < Level4Up_Score)
        m_level = GameScene::LEVEL4;
    else if (m_score >= Level4Up_Score)
        m_level = GameScene::LEVEL5;
    return m_level;
}

void NPC::Update()
{
    //call behaviac exec method
    this->btexec();
}