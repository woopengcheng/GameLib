#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include "behaviac/agent/agent.h"
#include "hero.h"
#include "NPC.h"
#include "enemy.h"
USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	bool bBlock = false;
	if (_tcsstr(lpCmdLine, _T("-block")))
	{
		bBlock = true;
	}

	behaviac::Config::SetSocketBlocking(bBlock);

    behaviac::Workspace::GetInstance()->SetFilePath("./behaviac/exported");
    behaviac::Agent::Register<NPC>();
    behaviac::Agent::Register<Hero>();
    behaviac::Agent::Register<Enemy>();
    //behaviac::TypeRegister::Register<NPC::refresh_delay>("NPC::refresh_delay");

    behaviac::Workspace::GetInstance()->ExportMetas("./behaviac/data.xml");

    //if (!cl.IsProfiling())
    //{
    //    behaviac::Agent::SetIdMask(kIdMask_Wolrd | kIdMask_Opponent);
    //}

    behaviac::Config::SetLogging(true);
    // create the application instance

    AppDelegate app;
    int ret = Application::getInstance()->run();
    behaviac::Agent::UnRegister<NPC>();
    behaviac::Agent::UnRegister<Hero>();
    behaviac::Agent::UnRegister<Enemy>();

    return ret;
}
