#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "PlaneHero.h"
#include "PlaneEnemy.h"
#include "GameOverScene.h"
#include <2d/CCSprite.h>
#include "NPC.h"
#include "hero.h"

using namespace CocosDenshion;

GameScene* GameScene::m_gamelayer = nullptr;

const float GameScene::refresh_delay[] = { 2.0f, 1.5f, 1.0f, 0.5f, 0.2f }; //战机刷新间隔
behaviac::vector<behaviac::Agent*>  GameScene::m_bt_agent_delete_queue;

Scene* GameScene::createScene()
{
    //创建一个没有重力的物理世界
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    //物理调试绘图
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    m_gamelayer = GameScene::create();
    scene->addChild(m_gamelayer);

    return scene;
}

bool GameScene::init()
{
    Layer::init();
    log("Game init!");

    //m_isAI = false;

    auto winSize = Director::getInstance()->getWinSize();

    //加载plist文件
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
    //加载音乐，播放背景音乐
    auto audioengine = SimpleAudioEngine::getInstance();
    audioengine->preloadBackgroundMusic("sound/bgmusic.mp3");
    audioengine->preloadEffect("sound/shoot.wav");
    audioengine->preloadEffect("sound/hero_down.mp3");
    audioengine->preloadEffect("sound/enemy3_down.wav");
    audioengine->preloadEffect("sound/enemy1_down.wav");
    audioengine->preloadEffect("sound/enemy2_down.wav");
    audioengine->preloadEffect("sound/big_spaceship_flying.mp3");

    audioengine->playBackgroundMusic("sound/bgmusic.mp3", true);
    //create NPC and hero
    m_NPC = behaviac::Agent::Create<NPC>();
    m_NPC->setParent(this);

    bool bOk = m_NPC->btload("npc");

    if (bOk)
    {
        m_NPC->btsetcurrent("npc");
    }
    else
    {
        printf("loading error!\n");
    }

    //this->SetIdFlag(1);
    m_Hero = behaviac::Agent::Create<Hero>();
    //播放背景图
    playBackground();

    //加入暂停按钮
    auto pause_sp1 = Sprite::createWithSpriteFrameName("game_pause_nor.png");
    auto pause_sp2 = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
    auto resume_sp1 = Sprite::createWithSpriteFrameName("game_resume_nor.png");
    auto resume_sp2 = Sprite::createWithSpriteFrameName("game_resume_pressed.png");

    auto pauseitem = MenuItemSprite::create(pause_sp1, pause_sp2);
    auto resumeitem = MenuItemSprite::create(resume_sp1, resume_sp2);
    auto pauseButton = MenuItemToggle::createWithTarget(this, menu_selector(GameScene::pauseButtonCallBack), pauseitem, resumeitem, nullptr);
    pauseButton->setAnchorPoint(Vec2(0, 0));
    pauseButton->setPosition(Vec2(winSize.width / 2 - pauseButton->getContentSize().width, winSize.height / 2 - pauseButton->getContentSize().height)); //位于左上角

    auto menu = Menu::create(pauseButton, nullptr);
    addChild(menu, 10, PAUSE_MENU);

    //create  AI  control button
    auto ai_enable1 = Sprite::create("img/check_box_active.png");
    auto ai_enable2 = Sprite::create("img/check_box_active_press.png");
    auto ai_enable3 = Sprite::create("img/check_box_normal.png");
    auto ai_enable4 = Sprite::create("img/check_box_normal_press.png");

    auto pause_ai_item = MenuItemSprite::create(ai_enable1, ai_enable2);
    auto resume_ai_item = MenuItemSprite::create(ai_enable3, ai_enable4);

    auto AIButton = MenuItemToggle::createWithTarget(this, menu_selector(GameScene::AIButtonCallBack), pause_ai_item, resume_ai_item, nullptr);
    AIButton->setAnchorPoint(Vec2(0, 0));
    AIButton->setPosition(Vec2(winSize.width / 3 - AIButton->getContentSize().width, winSize.height / 2 - AIButton->getContentSize().height));
    auto menu1 = Menu::create(AIButton, nullptr);
    addChild(menu1, 10, AI_TAG);

    //加入一个分数栏
    m_score = 0;
    auto scorelabel = Label::createWithSystemFont("Score: 0", "Arial", 36);
    scorelabel->setAnchorPoint(Vec2(0, 0));
    scorelabel->setColor(cocos2d::Color3B(255, 0, 0));
    scorelabel->setPosition(Vec2(0, winSize.height - scorelabel->getContentSize().height)); //位于右上角
    addChild(scorelabel, 10, SCORE_LABEL);
    //默认启动ＡＩ
    m_isAI = true;
    //加入我方战机，初始位置设定在下方中点
    auto hero = m_Hero->getPlane();
    addChild(hero, 0, HERO_TAG);
    hero->setPosition(Vec2(winSize.width / 2, hero->getContentSize().height / 2 + 10));
    //hero->setAI(m_isAI);
    m_Hero->setAI(m_isAI);

    //auto herobody = PhysicsBody::createBox(hero->getContentSize()); //这样设置不太精准
    auto herobody = PhysicsBody::create();
    Vec2 verts[] = { Vec2(0, 55), Vec2(50, -30), Vec2(-50, -30) };  //根据点组成一个多边形
    herobody->addShape(PhysicsShapeEdgePolygon::create(verts, 3));
    herobody->setCollisionBitmask(0x0); //不进行碰撞模拟
    herobody->setContactTestBitmask(HERO_CONTACTMASKBIT);
    hero->setPhysicsBody(herobody);

    //加入一个触摸监听，用来移动我方战机
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->setSwallowTouches(true);
    touchlistener->onTouchBegan = [this](Touch *pTouch, Event*) {
        //触摸着飞机移动
        auto hero = (PlaneHero*)getChildByTag(HERO_TAG);
        if (hero->boundingBox().containsPoint(pTouch->getLocation()))
        {
            //log("choose hero!");
            return true;
        }
        else return false;
    };
    touchlistener->onTouchMoved = [this](Touch* pTouch, Event*) {
        auto delta = pTouch->getDelta();
        auto hero = this->m_Hero->getPlane();
        auto oldpos = hero->getPosition(); //用来重置出界后的位置

        hero->setPosition(Vec2(hero->getPosition().x + delta.x, hero->getPosition().y + delta.y));

        //不能出界
        auto winSize = Director::getInstance()->getWinSize();
        if (hero->getPositionY() < hero->getContentSize().height / 2 || hero->getPositionY() > winSize.height - hero->getContentSize().height / 2
            || hero->getPositionX() < hero->getContentSize().width / 2 || hero->getPositionX() > winSize.width - hero->getContentSize().width / 2)
        {
            hero->setPosition(oldpos);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);

    //加入子弹渲染集合
    m_bulletBox = SpriteBatchNode::createWithTexture(hero->getTexture()); //这里获得的是一张纹理大图（整张png）
    addChild(m_bulletBox);

    //加入碰撞事件监听
    auto contactlistener = EventListenerPhysicsContact::create();
    contactlistener->onContactBegin = CC_CALLBACK_1(GameScene::dealWithContact, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactlistener, this);

    //开启Level检测，根据检测出的Level调整战机的刷新速度
    m_level = LEVEL1;
    schedule(schedule_selector(GameScene::testLevel), 1.0f);

    schedule(schedule_selector(GameScene::Update), 0.1f);

    //每隔5秒可以刷新一次boss战机
    //schedule(schedule_selector(GameScene::resetBoss), 5.0f);

    // 初始化并绑定
    auto listener = EventListenerKeyboard::create();
    //listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    //listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event)
    {
        log("Key with keycode %d pressed", keyCode);
        if (keyCode == EventKeyboard::KeyCode::KEY_S)
        {
            //物理调试绘图关
            this->getScene()->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
            this->m_Hero->getPlane()->enableShowScreenDebugInfo(false);
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_D)
        {
            //物理调试绘图开 
            this->getScene()->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
            this->m_Hero->getPlane()->enableShowScreenDebugInfo(true);
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	behaviac::Workspace::GetInstance()->SetFrameSinceStartup(0);
  
    return true;
}

void GameScene::playBackground()
{
    auto winSize = Director::getInstance()->getWinSize();

    int delta = 10; //补缝用的，两个背景紧挨着移动可能出现缝隙

    //播放背景，两个背景的图片是一样的，紧挨着循环播放
    //注意点：1.轮播图的像素高应该比设备的大一点 2.后面的图应该稍微往前靠一点 这样就不会有缝隙了
    auto bg1 = Sprite::createWithSpriteFrameName("background.png");
    bg1->setScaleX(winSize.width / bg1->getContentSize().width);
    bg1->setScaleY(winSize.height / bg1->getContentSize().height + 0.01);
    bg1->setAnchorPoint(Vec2(0, 0));
    addChild(bg1);

    auto move1 = MoveBy::create(30, Vec2(0, -winSize.height));
    auto callfunc1 = CallFunc::create([bg1](){
        bg1->setPosition(Vec2(0, 0));
    });
    auto action1 = Sequence::create(move1, callfunc1, nullptr);
    bg1->runAction(RepeatForever::create(action1));

    auto bg2 = Sprite::createWithSpriteFrameName("background.png");
    bg2->setScaleX(winSize.width / bg2->getContentSize().width);
    bg2->setScaleY(winSize.height / bg2->getContentSize().height + 0.01);
    bg2->setAnchorPoint(Vec2(0, 0));
    bg2->setPosition(Vec2(0, bg1->getPositionY() + winSize.height - delta));
    addChild(bg2);

    auto move2 = MoveTo::create(30, Vec2(0, 0));
    auto callfunc2 = CallFunc::create([bg2, bg1, winSize, delta](){
        bg2->setPosition(Vec2(0, bg1->getPositionY() + winSize.height - delta));
    });
    auto action2 = Sequence::create(move2, callfunc2, nullptr);
    bg2->runAction(RepeatForever::create(action2));
}

void GameScene::publishScore()
{
    auto userdefault = UserDefault::getInstance();

    //查看路径，测试用
    //log(userdefault->getXMLFilePath().c_str()); 

    //存储本次游戏分数
    char score_str[100] = { 0 };
    sprintf(score_str, "%d", m_score);
    userdefault->setStringForKey("LastScore", score_str);

    //存储最佳游戏分数
    auto bestscore = userdefault->getStringForKey("BestScore");
    if (m_score > atoi(bestscore.c_str()))
        userdefault->setStringForKey("BestScore", score_str);
}

void GameScene::gameover()
{
    unschedule(schedule_selector(GameScene::Update));
    unschedule(schedule_selector(GameScene::testLevel));
    publishScore(); //存储游戏分数
    cleanBehaviacAgentDeleteQueue();
    behaviac::Agent::Destroy(m_NPC);
    behaviac::Agent::Destroy(m_Hero);
    auto scene = TransitionFade::create(2.0, GameOverScene::createScene());
    Director::getInstance()->replaceScene(scene);
}

void GameScene::AIButtonCallBack(Ref* pSender)//点击AI按钮回调函数
{
    m_isAI = !m_isAI;
    m_Hero->setAI(m_isAI);
    //PlaneHero* hero = m_Hero->getPlane();
    //hero->setAI(m_isAI);

}

void GameScene::pauseButtonCallBack(Ref* pSender)
{
    //log("press pause button!");

    /*
     *  暂停/开启 步骤：
     *1.关闭/开启 触摸监听
     *2.遍历子节点，把除了暂停按钮的 关闭/开启 渲染(onExit)
     *3.关闭/开启 敌机的刷新schedule
     *4.关闭/开启 物理引擎的模拟
     */
    static int i = 0;
    if (0 == i)
    {
        i = 1;

        _eventDispatcher->pauseEventListenersForTarget(this);

        auto vec = this->getChildren();
        for (auto &child : vec)
        {
            if (child->getTag() != PAUSE_MENU)
                child->onExit();
        }

        unschedule(schedule_selector(GameScene::Update));

        ((Scene*)this->getParent())->getPhysicsWorld()->setAutoStep(false);

    }
    else
    {
        i = 0;

        _eventDispatcher->resumeEventListenersForTarget(this);

        auto vec = this->getChildren();
        for (auto &child : vec)
        {
            if (child->getTag() != PAUSE_MENU)
                child->onEnter();
        }

        schedule(schedule_selector(GameScene::Update), 0.1f);


        ((Scene*)this->getParent())->getPhysicsWorld()->setAutoStep(true);
    }
}

bool GameScene::dealWithContact(PhysicsContact& contact)
{
    //log("contact begin!");
    //获取两个碰撞的节点
    auto node1 = contact.getShapeA()->getBody()->getNode();
    auto node2 = contact.getShapeB()->getBody()->getNode();

    if (!node1 || !node2)
        return false;

    auto tag1 = node1->getTag();
    auto tag2 = node2->getTag();

    //如果碰撞的是子弹和敌方飞机
    if ((tag1 == HERO_BULLET_TAG && tag2 == ENEMY_TAG) || (tag2 == HERO_BULLET_TAG && tag1 == ENEMY_TAG))
    {
        PlaneEnemy* enemy = nullptr;
        if (tag1 == HERO_BULLET_TAG)
        {
            node1->removeFromParent();
            enemy = ((PlaneEnemy*)node2);
        }
        else
        {
            node2->removeFromParent();
            enemy = ((PlaneEnemy*)node1);
        }

        hitEnemy(enemy);
    }

    //如果碰撞的是我方战机和敌机
    if ((tag1 == HERO_TAG && tag2 == ENEMY_TAG) || (tag2 == HERO_TAG && tag1 == ENEMY_TAG))
    {
        _eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE); //不再接受触摸事件

        PlaneHero *hero = nullptr;
        PlaneEnemy *enemy = nullptr;
        if (tag1 == HERO_TAG)
        {
            hero = ((PlaneHero*)node1);
            enemy = ((PlaneEnemy*)node2);
        }
        else
        {
            hero = ((PlaneHero*)node2);
            enemy = ((PlaneEnemy*)node1);
        }

        hitEnemy(enemy);
        hitHero(hero);
    }

    return true;
}

void GameScene::hitEnemy(PlaneEnemy* enemy)
{
    enemy->getHurt();

    //如果敌方飞机挂了，增加分数
    if (!enemy->isLive())
    {
        m_score += enemy->getPoints();
        char buf[100] = { 0 };
        sprintf(buf, "Score: %d", m_score);
        auto scorelabel = (Label*)this->getChildByTag(SCORE_LABEL);
        scorelabel->setString(buf);
    }
}

void GameScene::hitHero(PlaneHero* hero)
{
    //我机直接死亡
    hero->dead();
}

void GameScene::levelUp(Level level)
{
    Label* label;
    int flontSize = 36;
    switch (level)
    {
    case LEVEL2:
        label = Label::createWithTTF("Good ! Level Up 2.", "fonts/Marker Felt.ttf", flontSize);

        break;
    case LEVEL3:
        label = Label::createWithTTF("Great ! Level Up 3.", "fonts/Marker Felt.ttf", flontSize);

        break;
    case LEVEL4:
        label = Label::createWithTTF("Awesome ! Level Up 4.", "fonts/Marker Felt.ttf", flontSize);

        break;
    case LEVEL5:
        label = Label::createWithTTF("Unbelievable ! Level Up 5.", "fonts/Marker Felt.ttf", flontSize);

        break;
    default:
        break;
    }

    auto s = Director::getInstance()->getWinSize();
    label->setPosition(Vec2(s.width / 2, s.height*0.6));

    this->addChild(label);
    label->setColor(Color3B::RED);

    //label->runAction(CCBlink::create(2, 10));
    label->runAction(
        Repeat::create(
        Sequence::create(
        CCBlink::create(2, 3),
        CCFadeTo::create(1, 0),
        nullptr), 1));
    //this->removeChild(label, true);
    //label->autorelease();
}

void GameScene::testLevel(float dt)
{
    int oldlevel = m_level;

    //根据到达的分数，获得等级
    oldlevel = m_NPC->getLevel(m_score);

    //如果level没变化，不需要更改刷新速度
    if (oldlevel == m_level)
        return;
    m_level = oldlevel;
    levelUp((Level)m_level);//升级
}

void GameScene::resetBoss(float dt)
{ 
    if (!m_NPC->m_canBossRefresh)
    {
        m_NPC->m_canBossRefresh = true;
    }
}

//create Update for behaviac
void GameScene::Update(float dt)
{
	behaviac::Workspace::GetInstance()->SetFrameSinceStartup(behaviac::Workspace::GetInstance()->GetFrameSinceStartup() + 1);

	//m_NPC->btexec();
	behaviac::Workspace::GetInstance()->Update();
    cleanBehaviacAgentDeleteQueue();
}

/******************************************************************
Function    : void GameScene::addBehaviacDeleteQueue(behaviac::Agent* pAgent)
Date        : 2015-11-13 16:42:24
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : add agent to the delete queue and delete it after Update 
******************************************************************/
void GameScene::addBehaviacAgentDeleteQueue(behaviac::Agent* pAgent)
{
    m_bt_agent_delete_queue.push_back(pAgent);
}

/******************************************************************
Function    : void GameScene::cleanBehaviacAgentDeleteQueue()
Date        : 2015-11-13 16:46:25
Author      : Quinn Pan
Parameter   : 
Return      : 
Desc        : remove all elements in the m_bt_agent_delete_queue
******************************************************************/
void GameScene::cleanBehaviacAgentDeleteQueue()
{
    for (auto &p : m_bt_agent_delete_queue)
    {
        p->SetActive(false);
        behaviac::Agent::Destroy(p);
    }
    m_bt_agent_delete_queue.clear();
}
