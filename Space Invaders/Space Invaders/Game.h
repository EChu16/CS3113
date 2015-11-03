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
#include "Bullet.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
#define NUM_ALIENS 15

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
	void LoadTitleScreen();
	void LoadGamePlay();
	void GameOver();

	//Game Continue
	bool gameEnded() const;

	//External Functions
	void LoadAllTexturesandSound();
	void DrawText(int fontTexture, std::string text, float size, float spacing);
	GLuint LoadTextureRGB(const char *image_path);
	GLuint LoadTextureRGBA(const char *image_path);
private:
	bool done;
	bool victory;
	float lastFrameTicks;
	SDL_Event event;
	SDL_Window* displayWindow;
	int numEnemiesAlive;
	float lastMonsterBullet;
	int randEnemy;
	
	//Matrices
	Matrix gameMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;
	ShaderProgram *program;

	//Sprite Sheet
	int spaceInvaderSprites;
	int fontSprites;

	//Textures
	SheetSprite playerImg;
	SheetSprite monsterImg;
	SheetSprite bulletImg;

	//Sounds
	Mix_Chunk *playerShoot;
	Mix_Chunk *monsterShoot;

	//Game States
	enum GAME_STATE { TITLESCREEN, GAMEPLAY, GAMEOVER };
	GAME_STATE state;

	//Entities for game
	Entity* player;
	std::vector<Entity*> enemies;
	std::vector<Bullet*> bullets;
};