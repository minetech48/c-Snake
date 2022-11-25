#include "gameSystem.h"
#include "engine/engineEvent.h"
#include "engine/gui/gui.h"

#include <stdio.h>

void GameSystem::initialize() {
	logf("Starting game system!\n");
	
	//events
	eventMap["GameRenderInit"] = GameRenderInit;
	
	GUIData::addStringHandler("gameScoreboard", scoreboardStringHandler);
	
	GUI::addSDLEventHandler(SDLEventHandler);
	
	Data::gridWidth = 30;
	Data::gridHeight = 20;
	
	Data::initialize();
}

void GameSystem::handleEvent(EngineEvent event) {
	if (eventMap.find(event.event) != eventMap.end())
		eventMap[event.event](event);
}

void GameSystem::update() {
	GR::renderFrame();
	
	GameData::tick();
}

void GS::SDLEventHandler(SDL_Event event) {
	switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				//snake controls
				case SDLK_w:
				case SDLK_UP:
					Data::queueAction(Up);
					break;
				case SDLK_s:
				case SDLK_DOWN:
					Data::queueAction(Down);
					break;
				case SDLK_a:
				case SDLK_LEFT:
					Data::queueAction(Left);
					break;
				case SDLK_d:
				case SDLK_RIGHT:
					Data::queueAction(Right);
					break;
				
				//game controls
				case SDLK_SPACE:
				case SDLK_RETURN:
					if (!Data::gameRunning)
						Data::initialize();
					break;
			}
			break;
			
		case SDL_WINDOWEVENT:
			if (event.window.event != SDL_WINDOWEVENT_SIZE_CHANGED) break;
		
		GR::windowResized();
	}
}

//events
void GS::GameRenderInit(EngineEvent event) {
	GameRenderer::init();
}

//strings
std::string GS::scoreboardStringHandler(std::string str) {
	std::string toReturn;
	
	toReturn+= "Score: " + std::to_string(Data::score);
	toReturn+= "\n Length: " + std::to_string(Data::snakeLength);
	toReturn+= "\n Next level: ";
	
	if (!Data::difficultyMap.empty())
		toReturn+=std::to_string(
			Data::difficultyMap.front().first-Data::snakeLength);
	else
		toReturn+= "Max";
	
	return toReturn;
}