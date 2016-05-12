#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include "cocos2d.h"

USING_NS_CC;

class GameOverScene : public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(GameOverScene);
	bool init();

private:
	void onBackItemCallback(Ref*);
};

#endif