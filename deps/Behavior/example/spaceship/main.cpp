/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "SDL.h"
#include "SDL_ttf.h"

#include "./framework/globals.h"
#include "./framework/GFrameHandler.h"
#include "./framework/Definitions.h"
#include "./framework/Ship.h"
#include "./framework/Projectile.h"
#include "./framework/WorldState.h"

#include "behaviac/base/core/profiler/profiler.h"

//only for cpp format
#include "behaviac_generated/behaviors/generated_behaviors.h"

#if BEHAVIAC_COMPILER_MSVC
#include <windows.h>

#if BEHAVIAC_COMPILER_MSVC2015
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
#endif
#endif


struct level_t
{
	const char*			name;
	const char*			module;
	framework::point	start;
	float				startRot;
	int					projectile;
	int					mode;
};

level_t gs_levels[] = {
	{"0_basic",					"drone",		framework::point(10.0f, 10.0f),		0.0f, 0, 0}, 
	{"1_1_suicide",				"Fighter1",		framework::point(400.0f, 10.0f),	0.0f, 1, 0}, 
	{"1_2_suicide",				"Drone1",		framework::point(400.0f, 10.0f),	0.0f, 1, 0}, 
	{"1_3_suicide",				"Fighter1",		framework::point(400.0f, 10.0f),	0.0f, 1, 0}, 
	{"2_basic_probability",		"Drone1",		framework::point(100.0f, 50.0f),	0.0f, 1, 0}, 
	{"3_basic_move",			"Fighter1",		framework::point(200.0f, 50.0f),	0.0f, 1, 0}, 
	{"4_destroy_projectiles",	"Fighter1",		framework::point(100.0f, 50.0f),	0.0f, 1, 0}, 
	{"5_group",					"drone",		framework::point(100.0f, 50.0f),	0.0f, 2, 2}, 
	{"6_director",				"",				framework::point(100.0f, 50.0f),	0.0f, 0, -1}, 
	{"7_signal",				"Sample Hull",	framework::point(100.0f, 50.0f),	-90.0f, 3, 0}, 
};

static const char* makeBoringBrain(framework::Projectile* s)
{
	const char* b = "base/boring";
	return b;
}

static const char* makeHomingBrain(framework::Projectile* s)
{
	const char* b = "base/homing";
	return b;
}

static const char* makeSlowBoringBrain(framework::Projectile* s)
{
	const char* b = "base/slowboring";
	return b;
}

static const char* makeSyncBrain(framework::Projectile* s)
{
	const char* b = "base/sync_homing";
	return b;
}

void loadlevel(int level)
{
	framework::WorldState* state = (framework::WorldState*)behaviac::Agent::GetInstance<framework::WorldState>();

	state->clearAIs();

	int levels = BEHAVIAC_ARRAY_NELEMENT(gs_levels);

	if (level >= 0 && level < levels)
	{
		framework::ws->GetPlayer()->setHealth(100);

		level_t& levelSt = gs_levels[level];

		char levelPath[_MAX_PATH];
		string_sprintf(levelPath, "ships/%s", levelSt.name);

		char levelModule[_MAX_PATH];
		string_sprintf(levelModule, "Hulls/%s", levelSt.module);

		if (levelSt.mode == 0)
		{
			framework::Ship* opponent = behaviac::Agent::Create<framework::Ship>();
			opponent->SetIdFlag(kIdMask_Opponent);
			opponent->displayHeading = 180;
			opponent->setSprite(levelModule);

			if (levelSt.projectile == 0)
			{
				opponent->projectileBrain = &makeBoringBrain;
			}
			else if (levelSt.projectile == 1)
			{
				opponent->projectileBrain = &makeHomingBrain;
			}
			else if (levelSt.projectile == 2)
			{
				opponent->projectileBrain = &makeSlowBoringBrain;
			}
			else if (levelSt.projectile == 3)
			{
				opponent->projectileBrain = &makeSyncBrain;
			}
			else if (levelSt.projectile == -1)
			{
				opponent->projectileBrain = 0;
			}
			else
			{
				BEHAVIAC_ASSERT(0);
			}

			opponent->btload(levelPath);
			opponent->btsetcurrent(levelPath);

			state->insertObject(opponent, levelSt.start);
			state->rotateObject(opponent, levelSt.startRot);

			framework::ws->SetAI(opponent);
		}
		else if (levelSt.mode > 0)
		{
			for (int i = 0 ; i < levelSt.mode ; i++)
			{
				for (int j = 0 ; j < levelSt.mode; j++)
				{
					framework::Ship* opponent = behaviac::Agent::Create<framework::Ship>();
					opponent->SetIdFlag(kIdMask_Opponent);

					opponent->displayHeading = 180;
					opponent->setSprite(levelModule);

					if (levelSt.projectile == 0)
					{
						opponent->projectileBrain = &makeBoringBrain;
					}
					else if (levelSt.projectile == 1)
					{
						opponent->projectileBrain = &makeHomingBrain;
					}
					else if (levelSt.projectile == 2)
					{
						opponent->projectileBrain = &makeSlowBoringBrain;
					}
					else
					{
						BEHAVIAC_ASSERT(0);
					}

					opponent->btload(levelPath);
					opponent->btsetcurrent(levelPath);

					state->insertObject(opponent, framework::point(levelSt.start.x + 50 * i, levelSt.start.y + 50 * j));
				}
			}

			framework::ws->SetAI(NULL);
		}
		else
		{
			state->btload(levelPath);
			state->btsetcurrent(levelPath);
			framework::ws->SetAI(NULL);
		}

		framework::ws->time = SDL_GetTicks();
		framework::ws->ResetSyncSignal();
	}
}

namespace framework
{
	void WorldState::SpawnShip(int level)
	{
		loadlevel(level);
	}
}//namespace framework

class MyCommandLine
{
	std::vector<std::string> argvs;

	bool	profiling;
	bool	block;
	int		level;

public:
	MyCommandLine(int argc, char** argv)
		: block(false), profiling(false), level(-1)
	{
		for (int i = 0; i < argc; ++i)
		{
			argvs.push_back(argv[i]);
		}
	}

	bool IsProfiling() const
	{
		return this->profiling;
	}

	bool IsBlocking() const
	{
		return this->block;
	}

	int Level() const
	{
		return this->level;
	}

	void PrintHelp()
	{
		printf("Usage: %s [OPTIONS]\n\n", argvs[0].c_str());

		printf("This program is a mini game to demo behavior tree.\n");
		printf("  -b  --block\t\tblock the start to wait for the connection\n");
		printf("  -l  --level=LEVEL\tspecify a level number\n");
		printf("  -p  --profile\t\tprofiling, to disable the connection to the designer\n");

		printf("\n");
		for (int i = 0; i < BEHAVIAC_ARRAY_NELEMENT(gs_levels); ++i)
		{
			printf("--level=%d, %s\n", i, gs_levels[i].name);
		}

		printf("\n");
	}

	void ReadOptions()
	{
		for (std::vector<std::string>::size_type i = 1; i < argvs.size(); ++i)
		{
			std::string arg = argvs[i];
			if (arg == "-b" || arg == "--block")
			{
				this->block = true;
				printf("waiting for the connecting...\n");
			}
			else if (arg == "-p" || arg == "--profile")
			{
				this->profiling = true;
				printf("profiling...\n");
			}
			else
			{
				size_t index = arg.find("-l=");
				std::string number;
				if (index == 0)
				{
					number = arg.substr(3);
				}
				else
				{
					index = arg.find("--level=");
					if (index == 0)
					{
						number = arg.substr(8);
					}
				}

				if (number.length() > 0)
				{
					int lv = atoi(number.c_str());
					if (lv >= 0 && lv < BEHAVIAC_ARRAY_NELEMENT(gs_levels))
					{
						this->level = lv;
						printf("level: %d  %s\n", this->level, gs_levels[level].name);
					}
				}
			}
		}
	}

	int SelectLevel()
	{
		int levels = BEHAVIAC_ARRAY_NELEMENT(gs_levels);
		int lev = -1;
		while (lev == -1)
		{
			printf("select a level: \n");

			int l;
			if (scanf("%d", &l) == 1)
			{
				if (l >= 0 && l < levels)
				{
					lev = l;
					printf("level: %d %s\n", lev, gs_levels[lev].name);
					break;
				}
				else
				{
					printf("%d is not a valid level number\n", l);
				}
			}
		}

		return lev;
	}
};

int g_level = -1;
int g_level_last = -1;
int g_gameover = 0;

static void SetDirectory()
{
#if BEHAVIAC_COMPILER_MSVC
	TCHAR szCurPath[_MAX_PATH];
	GetModuleFileName(NULL, szCurPath, _MAX_PATH);

	char* p = szCurPath;
	while(strchr(p,'\\'))
	{
		p = strchr(p,'\\');
		p++;
	}
	*p = '\0';

	SetCurrentDirectory(szCurPath);
#endif
}

static void HandleInput()
{
	framework::Ship* playerShip = framework::ws->GetPlayer();

	SDL_PumpEvents();
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_ESCAPE])
	{
		g_gameover = 1;
	}

	if (keystate[SDL_SCANCODE_S])
	{
		behaviac::State_t state;

		behaviac::Agent* pA = framework::ws->GetAI();
		pA->btsave(state);
		state.SaveToFile("btsave.xml");

		//state.LoadFromFile("btsave.xml");

		pA->btload(state);

		behaviac::State_t stateTemp;
		pA->btsave(stateTemp);
		stateTemp.SaveToFile("btsave2.xml");
	}

	if (keystate[SDL_SCANCODE_0])
	{
		if (BEHAVIAC_ARRAY_NELEMENT(gs_levels) > 0)
		{
			g_level = 0;
		}
	}
	else
	{
		for (int i = SDL_SCANCODE_1; i <= SDL_SCANCODE_9; ++i)
		{
			if (keystate[i])
			{
				int level = i - SDL_SCANCODE_1 + 1;
				if (level >= 0 && level < BEHAVIAC_ARRAY_NELEMENT(gs_levels))
				{
					g_level = level;
					break;
				}
			}
		}
	}

	if (keystate[SDL_SCANCODE_SPACE])
	{
		static int timeLastFired;

		if (framework::ws->time - timeLastFired > 30)
		{
			framework::Projectile* p = behaviac::Agent::Create<framework::Projectile>();
			p->SetIdFlag(kIdMask_Projectile);
			p->setSprite("Ammo/test");
			p->displayHeading = playerShip->displayHeading;
			p->setBrain(playerShip->projectileBrain(p));
			p->owner = playerShip;

			int w = playerShip->textureResource ? playerShip->textureResource->width : 0;
 			framework::ws->insertObject(p, playerShip->location.offsetRect(w * 0.3f, 0.0f));
		}

		timeLastFired = framework::ws->time;
	}

	if (keystate[SDL_SCANCODE_LSHIFT])
	{
		playerShip->accelMagnitude = 0;
	}

	if (keystate[SDL_SCANCODE_UP])
	{
		framework::ws->moveObject(playerShip, playerShip->location.offsetRect(0, -10));
	}
	if (keystate[SDL_SCANCODE_DOWN])
	{
		framework::ws->moveObject(playerShip, playerShip->location.offsetRect(0, 10));
	}
	if (keystate[SDL_SCANCODE_LEFT])
	{
		framework::ws->moveObject(playerShip, playerShip->location.offsetRect(-10, 0));
	}
	if (keystate[SDL_SCANCODE_RIGHT])
	{
		framework::ws->moveObject(playerShip, playerShip->location.offsetRect(10, 0));
	}
}

static void renderText(SDL_Renderer* sdlrenderer, TTF_Font* font, const char* text, int x, int y)
{
	const SDL_Color text_color = {255, 255, 255};
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, text_color);
	if (textSurface)
	{
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(sdlrenderer, textSurface);
		SDL_Rect dstrect;
		dstrect.x = x;
		dstrect.y = y;
		SDL_QueryTexture(textTexture, NULL, NULL, &dstrect.w, &dstrect.h);
		SDL_RenderCopy(sdlrenderer, textTexture, NULL, &dstrect);
	}
}

int main(int argc, char* argv[])
{
	SetDirectory();

	behaviac::IMemAllocator& allocator = behaviac::GetDefaultMemoryAllocator();
	size_t allocatedSize = allocator.GetAllocatedSize();

	MyCommandLine cl(argc, argv);
	cl.PrintHelp();
	cl.ReadOptions();

	g_level = cl.Level();
	if (g_level == -1)
	{
		g_level = cl.SelectLevel();
	}

	behaviac::Workspace::GetInstance()->SetFilePath("../example/spaceship/data/bt/exported");
	behaviac::Workspace::GetInstance()->SetFileFormat(behaviac::Workspace::EFF_xml);
	
	//LogManager::GetInstance()->SetFlush(true);
	behaviac::Agent::Register<framework::WorldState>();

	behaviac::Agent::RegisterInstanceName<framework::WorldState>();
	framework::WorldState* pWorldState = behaviac::Agent::Create<framework::WorldState>();

	pWorldState->SetIdFlag(kIdMask_World);

	behaviac::Agent::Register<framework::Ship>();
	behaviac::Agent::Register<framework::Projectile>();
	
	behaviac::Workspace::GetInstance()->ExportMetas("../example/spaceship/data/ships.xml");

	if (!cl.IsProfiling())
	{
		behaviac::Agent::SetIdMask(kIdMask_World | kIdMask_Opponent);
	}

	behaviac::Config::SetLogging(true);

	printf("game starting...\n");

	/* initialize SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init Failed : %s\n", SDL_GetError());
	}

	/* create window */
	SDL_Window* window = SDL_CreateWindow("Space Ship",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		kWIDTH,
		kHEIGHT,
		0);

	SDL_Surface* screen = SDL_GetWindowSurface(window);
	SDL_Renderer* sdlrenderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(sdlrenderer, 0, 0, 0, 255);

	// Initialize SDL_ttf library
	if (TTF_Init() != 0)
	{
		printf("TTF_Init Failed : %s\n", TTF_GetError());
	}

	// Load a font
	TTF_Font* font = TTF_OpenFont("../example/spaceship/Resources/Fonts/arial.ttf", 14);
	if (font == NULL)
	{
		printf("TTF_OpenFont Failed : %s\n", TTF_GetError());
		TTF_Quit();
	}

	framework::ws = behaviac::Agent::GetInstance<framework::WorldState>(0);

	framework::renderer = BEHAVIAC_NEW framework::Renderer();
	framework::renderer->sdlrenderer = sdlrenderer;

	framework::GameLogic* logic = BEHAVIAC_NEW framework::GameLogic();
	framework::gl = logic;

	GFrameHandler frame_handler;
	frame_handler.set_fps_limit(60);

	//background and player
	{
		framework::GameObject* background = BEHAVIAC_NEW framework::GameObject();
		background->usesPhysics = false;
		background->setSprite("Backgrounds/background", true);

		framework::ws->addBackgroundObject(background);

		framework::Ship* player = behaviac::Agent::Create<framework::Ship>();
		player->SetIdFlag(kIdMask_Player);
		player->setSprite("Hulls/Sample Hull");
		player->isPlayer = true;
		player->projectileBrain = &makeBoringBrain;

		framework::ws->insertObject(player, framework::point(100, 300));
		framework::ws->SetPlayer(player);
	}

	framework::ws->time = SDL_GetTicks();
	framework::ws->ResetSyncSignal();

	bool par_a = false;
	int par_b = 0;

	/* message pump */
	while (!g_gameover)
	{
		HandleInput();

		if (cl.IsProfiling())
		{
			behaviac::Profiler::GetInstance()->BeginFrame();
		}

		frame_handler.limit_frame();
		frame_handler.calculate();

		if (g_level != g_level_last)
		{
			g_level_last = g_level;

			loadlevel(g_level);

			char title[_MAX_PATH];
			string_sprintf(title, "%s - press '0-9' to switch levels", gs_levels[g_level].name);
			SDL_SetWindowTitle(window, title);

			par_a = !par_a;
			par_b++;

			if (framework::ws->GetAI())
			{
				framework::ws->GetAI()->SetVariable("par_a", par_a);
				framework::ws->GetAI()->SetVariable("par_b", par_b);
			}
		}

		logic->step();

		// clear the screen
		SDL_RenderClear(sdlrenderer);

		// render all sprites
		framework::renderer->Render();

		// Write text to surface
		if (framework::ws->GetPlayer())
		{
			char playerHP[_MAX_PATH];
			string_sprintf(playerHP, "Player HP : %d", framework::ws->GetPlayer()->getHealth());
			renderText(sdlrenderer, font, playerHP, kWIDTH - 120, kHEIGHT - 60);

			char aiHP[_MAX_PATH];
			if (framework::ws->GetAI())
			{
				string_sprintf(aiHP, "AI HP : %d", framework::ws->GetAI()->getHealth());
			}
			else
			{
				string_sprintf(aiHP, "AI Dead");
			}
			renderText(sdlrenderer, font, aiHP, kWIDTH - 120, kHEIGHT - 40);
		}

		// Refreshing the screen
		SDL_RenderPresent(sdlrenderer);

		if (cl.IsProfiling())
		{
			behaviac::Profiler::GetInstance()->EndFrame();
		}
	}

	/* cleanup SDL */
	//SDL_DestroyRenderer(sdlrenderer);
	//SDL_DestroyWindow(window);
	SDL_Quit();

	behaviac::Agent::UnRegister<framework::Projectile>();
	behaviac::Agent::UnRegister<framework::Ship>();
	behaviac::Agent::UnRegister<framework::WorldState>();
	behaviac::Agent::Destroy(pWorldState);

	behaviac::Agent::UnRegisterInstanceName<framework::WorldState>();


	size_t allocatedSize1 = allocator.GetAllocatedSize();
	size_t allocDiff = allocatedSize1 - allocatedSize;

	if (cl.IsProfiling())
	{
		const behaviac::string profile_data = behaviac::Profiler::GetInstance()->GetData(true, false);
		behaviac::string profile_data_m = "\n";
		profile_data_m += profile_data;
		profile_data_m += "\n";

		//BEHAVIAC_LOGMSG("\n%s\n", profile_data_m.c_str());
		behaviac::ConsoleOut::PrintLines(profile_data_m.c_str());
	}

	return 0;
}
