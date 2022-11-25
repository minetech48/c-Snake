#pragma once

#include <vector>
#include <list>
#include <queue>
#include <string>
#include <SDL2/SDL.h>

#define Data GameData


enum TileType {
	Empty,
	Snake,
	Apple,
	Wall
};

enum Direction {
	Up,
	Down,
	Left,
	Right
};

struct Tile {
	TileType tileType;
};

struct Point {int x, y;};

struct GameSettings {
	int gameSpeed;
	
	int maxApples, maxWalls, maxPortals;
};

class GameData {
	public:
		static int gridWidth, gridHeight;
			
		static std::vector<std::vector<Tile>> grid;
		
		static inline int tickNum = 0;
		
		static int snakeX, snakeY;
		static Direction snakeDir, queuedDir;
		static bool queueDir;
		static std::list<Point> snakeList;
		
		static int score, snakeLength;
		static int apples, walls, portals;
		
		static GameSettings currentSettings;
		static std::queue<std::pair<int, GameSettings>> difficultyMap;
		
		static bool gameRunning, gameInitialized;
		
		//game state functions
		static void initialize();
		static void tick();
		static void gameOver();
		
		static bool parseSettings(std::string filePath);
		
		//player actions
		static void queueAction(Direction dir);
		
		//data functions
		static TileType getTile(int x, int y);
		static void setTile(int x, int y, TileType type);
		
		static bool placeRandomTile(TileType type);
};
