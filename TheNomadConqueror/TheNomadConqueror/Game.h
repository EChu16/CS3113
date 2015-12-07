#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include "ShaderProgram.h"
#include "Matrix.h"

#include "Entity.h"
#include "SheetSprite.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#define LEVEL_HEIGHT 40
#define LEVEL_WIDTH 150
#define TILE_SIZE 1.0f
#define SPRITE_COUNT_X 10
#define SPRITE_COUNT_Y 10
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class Game {
public:
	//Game Loop
	Game();
	~Game();
	void Setup();
	void ProcessEvents(float elapsed);
	bool UpdateAndRender();
	void Render();
	void Update(float elapsed);

	//Game States
	enum GAME_STATE { TITLESCREEN, ALL_MAP_LEVELS, ABOUT, IN_GAME, VICTORY, PAUSE };
	enum LEVEL { NONE, FOREST, CANDYLAND, SNOW_TUNDRA, LAVALAND, CASTLE };
	enum TITLE_OPTIONS { PLAY, OPTIONS };
	GAME_STATE state;
	LEVEL level;
	TITLE_OPTIONS tOptions;

	//Game State Call Functions
	void LoadTitleScreen();
	void LoadStageMap();
	void RenderGame();
	void GameOver();

	//Load and Render Map Functions
	void LoadMap(std::string filename);
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readEntityData(std::ifstream &stream);
	void RenderBackground();
	void centerMap();
	void placeEntity(float startingXPos, float startingYPos, float startingDir, TYPE eType, SheetSprite& eSprite);

	//Load Levels Functions
	void LoadForestMap();
	void LoadCandylandMap();
	void LoadSnowTundraMap();

	//External Functions
	void LoadAllTexturesandSound();
	void DrawText(int fontTexture, std::string text, float size, float spacing);
	GLuint LoadTextureRGB(const char *image_path);
	GLuint LoadTextureRGBA(const char *image_path);
private:
	//Game essentials
	bool done;
	bool victory;
	float lastFrameTicks;
	SDL_Event event;
	SDL_Window* displayWindow;

	//Map variables
	int mapWidth;
	int mapHeight;
	unsigned char** bgLevelData;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;

	//For map loading
	std::string line, line2, tile, type, type_value, loc_value, data, data_value, xPosition, yPosition;
	float u, v, spriteWidth, spriteHeight;

	//Ingame vars
	LEVEL furthestLevel;
	LEVEL selectedLevel;
	std::vector<LEVEL> playable;

	//Matrices
	Matrix gameMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;
	ShaderProgram *program;

	//Sprite Sheet
	int fontSprites;
	int forestSprites;
	int candylandSprites;
	int snowtundraSprites;
	int playerSprites;

	//Images
	int bgID;
	int playID;
	int aboutID;
	int exitID;
	int mapLevelsID;

	//Sprites
	SheetSprite pStandingSprite;
	

	//Sounds
	Mix_Music* opening_music;
	Mix_Chunk* select_sound;
	Mix_Chunk* error_sound;

	//Mouse coordinates
	float unitX;
	float unitY;

	//Entities
	Entity* player;
	Entity* goldkey;
	Entity* ray_gun;
	std::vector<Entity*> solidTiles;
	std::vector<Entity*> enemies;
	std::vector<Entity*> coins;
};