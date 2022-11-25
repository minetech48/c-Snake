#include "gameSystem.h"

#include "engine/engineCore.h"
#include "engine/gui/gui.h"

#include <SDL2/SDL.h>

SDL_Renderer *renderer;

bool initialized = false;

int gameBorder = 10, borderX, borderY;
int tileSize = 10;

void GR::init() {
	renderer = GUI::getRenderer();
	
	windowResized();
	
	initialized = true;
	logf("Renderer initialized!\n");
}

void GR::renderFrame() {
	if (!initialized) return;
	if (renderer == NULL) {
		logf("ERROR, GameSystem SDL_Renderer is NULL, exiting!\n");
		EngineCore::running = false;
		return;
	}

	Renderer::start();
	
	drawTiles();
	drawGrid();
}

void GR::windowResized() {
	tileSize = std::min(
				(GUI::getWindowWidth() - gameBorder) / Data::gridWidth,
				(GUI::getWindowHeight() - gameBorder) / Data::gridHeight);
	
	borderX = (GUI::getWindowWidth() - tileSize * Data::gridWidth)/2;
	borderY = (GUI::getWindowHeight() - tileSize * Data::gridHeight)/2;
	
}

void GR::drawTiles() {
	SDL_Color color;
	SDL_Rect rect;
	
	for (int x = 0; x < Data::gridWidth; x++) {
		for (int y = 0; y < Data::gridHeight; y++) {
			rect = {x*tileSize + borderX, y*tileSize + borderY, tileSize, tileSize};
			
			switch (Data::getTile(x, y)) {
				case Snake:
					color = {0, 255, 0, 0};
					break;
				case Apple:
					color = {255, 0, 0, 0};
					break;
				case Wall:
					color = {0, 0, 0, 0};
					break;
				
				case Empty:
					continue;
			}
			
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void GR::drawGrid() {
	SDL_Color color = {200, 200, 200, 0};
	
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	
	for (int x = 0; x < Data::gridWidth+1; x++) {
		SDL_RenderDrawLine(renderer, 
			borderX + x*tileSize,
			borderY,
			borderX + x*tileSize,
			-borderY + GUI::getWindowHeight());
	}
	
	for (int y = 0; y < Data::gridHeight+1; y++) {
		SDL_RenderDrawLine(renderer, 
			borderX,
			borderY + y*tileSize,
			-borderX + GUI::getWindowWidth(),
			borderY + y*tileSize);
	}
}