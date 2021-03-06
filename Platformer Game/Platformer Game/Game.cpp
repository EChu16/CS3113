/*
Simple Platformer Game
Erich Chu
*/

#include "Game.h"

Game::Game() :done(false), lastFrameTicks(0.0f) {
	Setup();
}

Game::~Game() {
	delete program;
}

void Game::Setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program->programID);
	glViewport(0, 0, 640, 360);
	projectionMatrix.setOrthoProjection(-8.0f, 8.0f, -6.0f, 6.0f, -1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	program->setModelMatrix(gameMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);

	glClearColor(0.0f, 0.2f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	LoadAllTextures();
	LoadMap();
	RenderMap();
	centerMap();
	Render();

	SDL_GL_SwapWindow(displayWindow);
}

void Game::ProcessEvents(float elapsed) {
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	if (keys[SDL_SCANCODE_LEFT]) {
		if (player->getDirection() > 0) {
			player->changeXAcc(0);
		}
		player->changeXAcc(-3.0F);
		player->updateVals(elapsed);
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		if (player->getDirection() < 0) {
			player->changeXAcc(0);
		}
		player->changeXAcc(3.0F);
		player->updateVals(elapsed);
	}
	else {
		player->changeXAcc(0);
		player->updateVals(elapsed);
	}
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else {
			player->changeXAcc(0.0f);
			player->updateVals(elapsed);
		}
	}
}

bool Game::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	ProcessEvents(elapsed);
	Update(elapsed);
	Render();
	return done;
}

void Game::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	RenderGame();
	centerMap();
}

void Game::Update(float elapsed) {

}

void Game::placeEntity(std::string type, float xPos, float yPos)
{
	if (type == "start") {
		player = new Entity(xPos, yPos, 10.0f, 10.0f, type);
	}
}

void Game::centerMap() {
	viewMatrix.identity();
	viewMatrix.Translate(-player->getxPos(), -player->getYPos(), 0.0f);
	program->setViewMatrix(viewMatrix);
}

void Game::RenderMap() {
	//Draw Map
	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
			float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
			float spriteWidth = 0.37f / (float)SPRITE_COUNT_X;
			float spriteHeight = 0.9f / (float)SPRITE_COUNT_Y;
			vertexData.insert(vertexData.end(), {
				TILE_SIZE * x, -TILE_SIZE * y,
				TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
				(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
				TILE_SIZE * x, -TILE_SIZE * y,
				(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
				(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
			});
			texCoordData.insert(texCoordData.end(), {
				u, v,
				u, v + (spriteHeight),
				u + spriteWidth, v + (spriteHeight),
				u, v,
				u + spriteWidth, v + (spriteHeight),
				u + spriteWidth, v
			});
		}
	}
}

void Game::RenderGame() {
	//Draw Map
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	gameMatrix.identity();
	program->setModelMatrix(gameMatrix);
	glBindTexture(GL_TEXTURE_2D, tileSprites);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 50 * 30);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	//Draw Player
	player->Draw(program, gameMatrix, playerImg);
	SDL_GL_SwapWindow(displayWindow);
}

void Game::LoadMap() {
	std::ifstream infile("maps/map_platform.txt");
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[ObjectLayer]") {
			readEntityData(infile);
		}
	}

	infile.close();
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 154) {
				solidBlocks.push_back(new Entity((TILE_SIZE * x) + (TILE_SIZE / 2.0), (-TILE_SIZE * y) - (TILE_SIZE / 2.0), 0.5f, 0.5f, "block"));
			}
		}
	}
}

//read map headers
bool Game::readHeader(std::ifstream &stream) {
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		std::istringstream sStream(line);
		std::string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}
	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

//read map layer
bool Game::readLayerData(std::ifstream &stream) {
	while (getline(stream, line)) {
		if (line == "") { break; }
		std::istringstream sStream(line);
		getline(sStream, key, '=');
		getline(sStream, value);
		getline(stream, line2);
		std::istringstream sStream2(line2);
		getline(sStream2, key2, '=');
		getline(sStream2, value2);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				std::istringstream lineStream(line);
					getline(lineStream, tile, ',');
				for (int x = 0; x < mapWidth; x++) {
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

//Read map entity data
bool Game::readEntityData(std::ifstream &stream) {
	while (getline(stream, line)) {
		if (line == "") { break; }
		std::istringstream sStream(line);
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			std::istringstream lineStream(value);
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			float placeX = (float)atoi(xPosition.c_str()) * TILE_SIZE * 2.0f;
			float placeY = (float)atoi(yPosition.c_str()) * -TILE_SIZE * 1.0f;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

//Load all textures
void Game::LoadAllTextures() {
	tileSprites = LoadTextureRGBA("images/tiles_spritesheet.png");
	playerSprites = LoadTextureRGBA("images/p1_spritesheet.png");
	playerImg = SheetSprite(playerSprites, 146.0f / 508.0f, 0.0f / 208.0f, 72.0f / 508.0f, 70.0f / 208.0f, 2.0f);
}
//Load JPG files
GLuint Game::LoadTextureRGB(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
};

//Load PNG files
GLuint Game::LoadTextureRGBA(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
};


void Game::DrawText(int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
};