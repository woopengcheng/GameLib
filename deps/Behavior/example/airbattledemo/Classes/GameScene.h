#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"
#include "behaviac/base/core/container/vector_t.h"
USING_NS_CC;

class PlaneEnemy;
class PlaneHero;
class NPC;
class Hero;
namespace behaviac{
    class Agent;
}
//游戏主场景层
class GameScene : public Layer
{
    //以下是游戏配置信息
public:
    enum NodeTag
    {
        HERO_TAG = 100,
        ENEMY_TAG = 101,
        HERO_BULLET_TAG = 102,
        SCORE_LABEL = 103,
        PAUSE_MENU = 104,
        AI_TAG = 105,
    };

    //碰撞事件bit位
    enum ContactMaskBit
    {
        ENEMY_CONTACTMASKBIT = 0x01 || 0x02,
        HERO_BULLET_CONTACTMASKBIT = 0x01,
        HERO_CONTACTMASKBIT = 0x02
    };

    //游戏level，分数越高level越高
    enum Level
    {
        LEVEL1,
        LEVEL2,
        LEVEL3,
        LEVEL4,
        LEVEL5
    };

    ////level提升所需的分数
    //enum LevelUp_Score
    //{
    //       Level1Up_Score = 0,
    //	Level2Up_Score = 200,
    //	Level3Up_Score = 500,
    //	Level4Up_Score = 1000
    //};

    static const float refresh_delay[]; //战机刷新间隔

public:
    static Scene* createScene();
    CREATE_FUNC(GameScene);
    bool init();

public:
    static GameScene* sharedGameLayer() { return m_gamelayer; } //获取该游戏层单例对象

private:
    static GameScene* m_gamelayer; //游戏场景层的单例对象
public:
    SpriteBatchNode* getBulletBox() { return m_bulletBox; }	 //获取子弹渲染器
    void AdjustHeroPosition(Node* hero);
    void gameover();
private:
    SpriteBatchNode *m_bulletBox; //存放子弹的渲染器

private:
    int m_level; //当前游戏level
    int m_score; //当前游戏分数

private:
    void playBackground(); //创建并移动游戏背景
    void publishScore(); //存储游戏分数

    void pauseButtonCallBack(Ref* pSender); //点击暂停按钮回调函数
    void AIButtonCallBack(Ref* pSender); //点击AI按钮回调函数

    bool dealWithContact(PhysicsContact&); //物理碰撞事件回调函数
    void hitEnemy(PlaneEnemy* enemy);
    void hitHero(PlaneHero* hero);

    void levelUp(Level level);//升级动画
    //游戏中schedule的回调函数
    void testLevel(float dt); //每秒调用一次，根据当前到达的分数设置游戏level
    //设置hero Plane's position
    //void setHeroPlaneUnderPosition(CCPoint _targetPosition);
    //m_isAI表示是否使用AI
    bool m_isAI;
    //m_canBossRefresh表示可否刷新boss，每隔dt秒可以刷新一次
    void resetBoss(float dt);
    NPC * m_NPC;
    Hero* m_Hero;
public:
    // //create Update for behaviac
    void Update(float dt);
    static void addBehaviacAgentDeleteQueue(behaviac::Agent* pAgent);
private:
    static void cleanBehaviacAgentDeleteQueue();
private:
    /******************************************************************
    Date        : 2015-11-13 16:40:05
    Author      : Quinn Pan
    Desc        : add some behaviac into the queue , and delete it
    ******************************************************************/
    static behaviac::vector<behaviac::Agent*> m_bt_agent_delete_queue;
};

#endif

