#include "behaviac/agent/agent.h"
#include "behaviac/agent/registermacros.h"
#pragma once

class PlaneEnemy;
class NPC :
    public behaviac::Agent
{
public:
    DECLARE_BEHAVIAC_OBJECT(NPC, behaviac::Agent);

    //typedef struct  refresh_delay
    //{
    int level0_daly = 20;
    int level1_daly = 15;
    int level2_daly = 10;
    int level3_daly = 5;
    int level4_daly = 2;
    //    void reset()
    //    {

    //    }

    //    DECLARE_BEHAVIAC_OBJECT_STRUCT(NPC::refresh_delay);
    //}refresh_delay_type;

    //level提升所需的分数
    int  Level1Up_Score = 0;
    int  Level2Up_Score = 200;
    int  Level3Up_Score = 500;
    int  Level4Up_Score = 1000;
    NPC();
    ~NPC();
    NPC(void* parent);
    void initData();
    void spawnEnemy();
    void* getParent();
    // set NPC parent
    void setParent(const void* parent);
    void setLeveUpScore(int level, int score);
    int getGameLevel();
    // set the game current level
    int getLevel(int score);

    //create method for behaviac
    void Update();


private:
    void* m_parent;

   
public:

    bool m_canBossRefresh;
    int m_level;
};
