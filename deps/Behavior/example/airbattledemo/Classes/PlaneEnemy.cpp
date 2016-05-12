#include "PlaneEnemy.h" 
#include "SimpleAudioEngine.h"
#include "enemy.h"
using namespace CocosDenshion;

PlaneEnemy* PlaneEnemy::createWithEnemyType(int planetype)
{
	//自定义一个工厂方法，根据传入的战机类型，初始化敌方战机
	auto plane = new PlaneEnemy();
	if (plane && plane->init(planetype))
	{
		plane->autorelease();
		return plane;
	}
	else
	{
		CC_SAFE_DELETE(plane);
		return nullptr;
	}
}
/*
敌机数据初始化
*/
bool PlaneEnemy::init(int planetype)
{
	Sprite::init();

	m_planeType = planetype;
	m_live = true;
	
	//初始化纹理图，战机速度，生命，分值
	String framename;
	switch (planetype)
	{
	case EnemyPlaneType::Enemy1:
		framename = "enemy1.png";
		m_velocity = random(0.5, 1.5) * Enemy1_Vec; //设置一个随机速度
		m_life = Enemy1_Life;
		m_points = Enemy1_Points;
		break;
	case EnemyPlaneType::Enemy2:
		framename = "enemy2.png";
		m_velocity = random(0.5, 1.5) * Enemy2_Vec;
		m_life = Enemy2_Life;
		m_points = Enemy2_Points;
		break;
	case EnemyPlaneType::Enemy3:
		framename = "enemy3_n1.png";
		m_velocity =  random(0.5, 1.5) * Enemy3_Vec;
		m_life = Enemy3_Life;
		m_points = Enemy3_Points;
		break;
	case EnemyPlaneType::Enemy4:
		framename = "enemy3_n2.png";
		m_velocity = random(0.5, 1.5) * Enemy4_Vec;
		m_life = Enemy4_Life;
		m_points = Enemy4_Points;
		break;
	}
	initWithSpriteFrameName(framename.getCString());

	//加载敌人爆炸精灵帧集合
	initEnemyBlowUpFrames(planetype);

	return true;
}

void PlaneEnemy::onEnter()
{
	Sprite::onEnter();

	schedule(schedule_selector(PlaneEnemy::moveOn)); //战机进入后，开始移动
}
/*
添加爆炸的事件
*/
void PlaneEnemy::blowUp()
{
	//根据战机类型播放音效
	switch (m_planeType)
	{
	case Enemy1:
		SimpleAudioEngine::getInstance()->playEffect("sound/enemy1_down.wav");
		break;
	case Enemy2:
		SimpleAudioEngine::getInstance()->playEffect("sound/enemy2_down.wav");
		break;
	case Enemy3:
		SimpleAudioEngine::getInstance()->playEffect("sound/enemy3_down.wav");
		break;
	case Enemy4:
		SimpleAudioEngine::getInstance()->playEffect("sound/enemy3_down.wav");
		break;
	}

	//加载死亡动画
	auto animation = Animation::createWithSpriteFrames(m_blowframes);
	animation->setDelayPerUnit(0.2);
	animation->setRestoreOriginalFrame(true);
	auto blowUp = Animate::create(animation);

	//爆炸完后清除
	auto clear = CallFunc::create([this]() {
        //set the agent's m_plane is null
        
		removeFromParent();
		//log("enemy cleared!");
	});
    //将代理的中指向自己的指针去掉
    this->myAgent->setPlane(nullptr);

	this->runAction(Sequence::create(blowUp, clear, nullptr));
}

//set the PlaneEnemy's speed
void PlaneEnemy::setSpeed(float speed)
{
    this->m_velocity = (int)speed;
}

/*get the PlaneEnemy's speed*/
float PlaneEnemy::getSpeed()
{
    return this->m_velocity;
}


void PlaneEnemy::getHurt()
{
	m_life--;
	if (m_life <= 0)
	{
		m_live = false;
		getPhysicsBody()->setContactTestBitmask(0x0); ////设置碰撞标志位，不再发生碰撞事件
		blowUp(); //播放爆炸动画
		return;
	}
	else if (m_planeType != Enemy1) //初级战机没有受伤动画
	{
		//播放受伤动画
		SpriteFrame *hurt = nullptr;
		SpriteFrame *old = nullptr;
		switch (m_planeType)
		{
		case Enemy2:
			hurt = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_hit.png");
			old = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png");
			break;
		case Enemy3:
			hurt = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_hit.png");
			old = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png");
			break;
		case Enemy4:
			hurt = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_hit.png");
			old = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png");
			break;
		}

		auto setHurtImg = CallFunc::create([this, hurt](){
			this->setSpriteFrame(hurt);
		});

		auto setOldImg = CallFunc::create([this, old](){
			this->setSpriteFrame(old);
		});

		auto hurtAction = Sequence::create(setHurtImg, DelayTime::create(0.2), setOldImg, nullptr);

		this->stopAllActions();
		this->runAction(hurtAction);
	}
}
/*
初始化每一种战机爆炸的效果
*/
void PlaneEnemy::initEnemyBlowUpFrames(int planetype)
{
	if (planetype == EnemyPlaneType::Enemy1)
	{
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down1.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down2.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down3.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down4.png"));
	}
	
	else if (planetype == EnemyPlaneType::Enemy2)
	{
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down1.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down2.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down3.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down4.png"));
	}

	else
	{
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down1.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down2.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down3.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down4.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down5.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down6.png"));
	}
}

void PlaneEnemy::moveOn(float dt)
{
	//log("enemy moveon!");
    m_velocity =(int) this->myAgent->speed;
	//根据行进速度，往下移动，一旦出界，清除出去
	this->setPositionY(getPositionY() - dt * m_velocity);
	if (getPositionY() < -this->getContentSize().height / 2)
	{
		removeFromParent();
		//log("enemy out!");
	}
}