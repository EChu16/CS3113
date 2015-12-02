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
	enum LEVEL { ONE, TWO, THREE, FOUR, FIVE };
	enum TITLE_OPTIONS { PLAY, OPTIONS };
	GAME_STATE state;
	LEVEL level;
	TITLE_OPTIONS tOptions;

	//Game State Call Functions
	void LoadTitleScreen();
	void LoadStageMap();
	void GameOver();

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


	//Ingame vars
	LEVEL furthestLevel;

	//Matrices
	Matrix gameMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;
	ShaderProgram *program;

	//Sprite Sheet
	int fontSprites;

	//Images
	int bgID;
	int playID;
	int aboutID;
	int exitID;
	int mapLevelsID;

	//Textures
	SheetSprite playerImg;
	SheetSprite monsterImg;
	SheetSprite bulletImg;

	//Sounds
	Mix_Music* opening_music;
	Mix_Chunk* select_sound;

	//Mouse coordinates
	float unitX;
	float unitY;

};