#include <stdio.h>

#include <engine/engineCore.h>
#include <engine/gui/gui.h>
#include <game/gameSystem.h>

#include "engine/logger.h"

int main(int argc, char* args[]) {
	Logger::init();
	

	GUI::WINDOW_NAME = "Snake! i3-float";
	EngineCore::initSystem(new GUI());
	EngineCore::initSystem(new GameSystem());
	
	EngineCore::broadcast("GUISetTheme", "gui/theming/DefaultTheme");
	EngineCore::broadcast("GUIShow", "gui/MainMenu");
	
	EngineCore::broadcast("GameRenderInit");
	
	EngineCore::start();
	
	return 0;
}