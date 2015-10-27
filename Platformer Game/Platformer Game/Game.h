#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.h"
#include "SheetSprite.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#define TILE_SIZE 1.0f
#define LEVEL_HEIGHT 1.0f
#define LEVEL_WIDTH 3.0f
#define SPRITE_COUNT_X 13
#define SPRITE_COUNT_Y 13

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
	void LoadMap();
	void RenderGame();
	
	//Map functions
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readEntityData(std::ifstream &stream);
	void placeEntity(std::string type, float xPos, float yPos);

	//External Functions
	void LoadAllTextures();
	void DrawText(int fontTexture, std::string text, float size, float spacing);
	GLuint LoadTextureRGB(const char *image_path);
	GLuint LoadTextureRGBA(const char *image_path);
private:
	bool done;
	float lastFrameTicks;
	SDL_Event event;
	SDL_Window* displayWindow;

	//Matrices
	Matrix gameMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;
	ShaderProgram *program;

	//Map
	int mapWidth;
	int mapHeight;

	//Reading map file
	std::string line, key, value, tile, type, xPosition, yPosition;

	unsigned char** levelData;

	//Sprite Sheet
	int tileID;
	int tileSprites;
	int playerSprites;

	//Textures
	SheetSprite playerImg;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;

	//Entities for game
	Entity* player;
};