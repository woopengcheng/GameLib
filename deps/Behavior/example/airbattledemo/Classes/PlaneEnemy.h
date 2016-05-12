#ifndef PLANEENEMY_H
#define PLANEENEMY_H

#include "cocos2d.h"

USING_NS_CC;
class Enemy;
//敌方飞机类
class PlaneEnemy : public Sprite
{
public:
	enum EnemyPlaneType
	{
		Enemy1,
		Enemy2,
		Enemy3,
		Enemy4
	}; //战机类型

	enum EnemyPlaneVec
	{
		Enemy1_Vec = 150,
		Enemy2_Vec = 100,
		Enemy3_Vec = 50,
		Enemy4_Vec = 50
	}; //战机速度

	enum EnemyLife
	{
		Enemy1_Life = 1,
		Enemy2_Life = 2,
		Enemy3_Life = 5,
		Enemy4_Life = 8
	}; //战机生命

	enum Enemy_Points
	{
		Enemy1_Points = 10,
		Enemy2_Points = 20,
		Enemy3_Points = 50,
		Enemy4_Points = 80
	}; //战机分数

    Enemy* myAgent;
public:
	//自定义工厂方法，由于enemy有很多种，根据enemy的类型初始化，在init里面进行初始化
	static PlaneEnemy* createWithEnemyType(int planetype);
	bool init(int planetype);

	void onEnter();

	void getHurt(); //飞机被击中一次
	bool isLive() { return m_live; }; 
	int getPoints() { return m_points; }
	int getType() { return m_planeType; }
    void setSpeed(float speed);
    float getSpeed();
private:
	void initEnemyBlowUpFrames(int planetype); //加载爆炸纹理图集
	void moveOn(float dt); //schedule回调，每dt秒移动一段距离
	void blowUp(); //飞机爆炸
    
private:
	int m_velocity; //战机速度
	int m_life; //战机当前生命
	int m_points; //战机分值
	int m_planeType; //敌机机型
	bool m_live; //飞机是否活着的标志

	Vector<SpriteFrame*> m_blowframes; //存放爆炸纹理精灵帧
};

#endif

