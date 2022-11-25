#include "gameData.h"

#include <engine/util/fileIO.h>
#include <engine/logger.h>
#include <engine/math.h>

#include <ctime>
#include <fstream>
#include <string>

int Data::gridWidth;
int Data::gridHeight;

std::vector<std::vector<Tile>> Data::grid;

int Data::snakeX, Data::snakeY;
Direction Data::snakeDir, Data::queuedDir;
bool Data::queueDir;
std::list<Point> Data::snakeList;

int Data::score, Data::snakeLength;
int Data::apples, Data::walls;

GameSettings Data::currentSettings;
std::queue<std::pair<int, GameSettings>> Data::difficultyMap;

bool Data::gameRunning = false, Data::gameInitialized = false;

//game state functions
void Data::initialize() {
	//clearing difficultyMap
	while (!difficultyMap.empty()) difficultyMap.pop();
	
	if (!parseSettings("game/mapDifficulty")) {
		logf("Game init failure, aborting!\n");
		return;
	}
	currentSettings = difficultyMap.front().second;
	
	grid = std::vector<std::vector<Tile>>(gridWidth, std::vector<Tile>(gridHeight, Tile()));
	
	apples = 0;
	walls = 0;
	
	snakeX = gridWidth/2;
	snakeY = gridHeight/2;
	snakeDir = Right;
	queueDir = false;
	score = 0;
	
	snakeLength = difficultyMap.front().first;
	snakeList.clear();
	
	setTile(snakeX, snakeY, Snake);
	snakeList.emplace_front(Point {snakeX, snakeY});
	
	srand((unsigned) time(0));
	
	difficultyMap.pop();
	
	gameInitialized = true;
	logf("Game Initialized!\n");
}

void Data::tick() {
	if (!gameRunning || tickNum++ % currentSettings.gameSpeed != 0) return;
	
	switch (snakeDir) {
		case Up:
			snakeY--;
			break;
		case Down:
			snakeY++;
			break;
		case Left:
			snakeX--;
			break;
		case Right:
			snakeX++;
			break;
	}
	
	//checking snake's new tile for actions
	if (getTile(snakeX, snakeY) == Wall || getTile(snakeX, snakeY) == Snake) {
		gameOver();
		return;
	}
	if (getTile(snakeX, snakeY) == Apple) {apples--; snakeLength++; score++;
		if (snakeLength == difficultyMap.front().first) {
			currentSettings = difficultyMap.front().second;
			difficultyMap.pop();
		}
	}
	
	//adding to snake list
	setTile(snakeX, snakeY, Snake);
	snakeList.emplace_front(Point {snakeX, snakeY});
	
	//removing from snake list
	if (snakeList.size() > snakeLength) {
		setTile(snakeList.back().x, snakeList.back().y, Empty);
		
		snakeList.pop_back();
	}
	
	//regenerating tiles (apples)
	if (apples < currentSettings.maxApples) if (placeRandomTile(Apple)) apples++;
	if (walls < currentSettings.maxWalls) if (placeRandomTile(Wall)) walls++;
	
	//advancing control queue if applicable
	if (queueDir) {
		snakeDir = queuedDir;
		queueDir = false;
	}
}

void Data::gameOver() {
	gameInitialized = false;
	gameRunning = false;
	
	logf("Game Over!\n");
	logf("Score: %d\n", score);
}

//player actions
bool isLegalMove(Direction from, Direction to) {
	if (from == Up && to == Down) return false;
	if (from == Down && to == Up) return false;
	if (from == Left && to == Right) return false;
	if (from == Right && to == Left) return false;
	
	return true;
}
void Data::queueAction(Direction dir) {
	if (!gameRunning) {
		if (gameInitialized) {
			snakeDir = dir;
			gameRunning = true;
			logf("Game Started!\n");
		}
		return;
	}
	
	if (!queueDir && !isLegalMove(snakeDir, dir)) return;
	
	if (queueDir && isLegalMove(snakeDir, dir)) {
		queuedDir = dir;
	}else {
		snakeDir = dir;
		queuedDir = dir;
		queueDir = true;
	}
}

//data functions
bool Data::placeRandomTile(TileType type) {
	int x = rand() % gridWidth;
	int y = rand() % gridHeight;
	
	if (getTile(x, y) != Empty) return false;
	
	setTile(x, y, type);
	return true;
}


void Data::setTile(int x, int y, TileType type) {
	grid[x][y].tileType = type;
}

TileType Data::getTile(int x, int y) {
	if (x >= gridWidth || x < 0) return Wall;
	if (y >= gridHeight || y < 0) return Wall;
	
	return grid[x][y].tileType;
}


//loading mapDifficutly file
using std::stoi;
bool Data::parseSettings(std::string filePath) {
	std::ifstream file(FileIO::findFile(filePath, ".txt"));
	
	if (!file.is_open()) {
		logf("Failed to load game settings, file not found!\n");
		return false;
	}
	
	std::string line;
		std::getline(file, line);
	int delimiterIndex = line.find_first_of(',');
	
	//getting grid dimensions from the top of the file
	gridWidth = stoi(line.substr(0, delimiterIndex));
	gridHeight= stoi(line.substr(delimiterIndex+2));
	
	//parsing file
	//persistent struct data
	int score = 0;
	int apples, walls, portals, speed;
	
	//struct
	GameSettings data;
	
	//parsing data
	std::string key;
	int value;
	
	while (std::getline(file, line)) {
		if (line.size() == 0) continue;
		
		logf("%s\n", line.c_str());
		if ((delimiterIndex = line.find_first_of(',')) == std::string::npos) {
			if (score != 0) {
				logf("score!: %d\n", score);
				difficultyMap.push({score, data});
			}
			score = stoi(line);
			continue;
		}
		
		key = line.substr(0, delimiterIndex);
		value = stoi(line.substr(delimiterIndex+2));
		
		switch (hash(key)) {
			case hash("speed"):
				data.gameSpeed = value;	break;
			case hash("apples"):
				data.maxApples = value;	break;
			case hash("walls"):
				data.maxWalls = value;	break;
			case hash("portals"):
				data.maxPortals = value;break;
		}
	}
	difficultyMap.push({score, data});
	
	return true;
}