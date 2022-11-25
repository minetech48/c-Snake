#pragma once

#include <string>
#include <map>
#include <functional>

#include "engine/engineSystem.h"
#include "engine/engineEvent.h"
#include "engine/logger.h"

#include "gameData.h"

#define GS GameSystem
#define GR GameRenderer

static std::map<std::string, std::function<void(EngineEvent)>> eventMap;

class GameSystem : public EngineSystem {
	
	static std::string currentBoard;
	
	public:
		GameSystem() {title = "GameSystem";}
		
		void initialize();
		void handleEvent(EngineEvent event);
		void update();
		
		static void SDLEventHandler(SDL_Event event);
		
		//events
		static void GameRenderInit(EngineEvent event);
		
		//strings
		static std::string scoreboardStringHandler(std::string str);
};

class GameRenderer{
	public:
		static void init();
		
		static void renderFrame();
		
		static void drawTiles();
		static void drawGrid();
		
		static void windowResized();
};