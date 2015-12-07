/*
Space Invaders
Erich Chu
*/

#include "Game.h"

Game::Game() :done(false), lastFrameTicks(0.0f) {
	Setup();
}

Game::~Game() {
	Mix_FreeMusic(opening_music);
	Mix_FreeChunk(select_sound);
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
	projectionMatrix.setOrthoProjection(-20.0f,20.0f,-30.0f,30.0f, -1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	program->setModelMatrix(gameMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);

	state = TITLESCREEN;
	furthestLevel = FOREST;
	selectedLevel = NONE;
	
	//glClearColor(0.64902f, 0.747059f, 0.847059f, 1.0f);	//Snow Tundra
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	LoadAllTexturesandSound();
	Mix_PlayMusic(opening_music, -1);
	Render();

	SDL_GL_SwapWindow(displayWindow);
}

void Game::LoadTitleScreen() {
	//Texture Coords
	float texCoords[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	//Background Image for Title Screen
	float bg_vertices[] = { -20.0f, -30.0f, 20.0f, 30.0f, -20.0f, 30.0f, 20.0f, 30.0f, -20.0f, -30.0f, 20.0f, -30.0f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, bg_vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, bgID);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	//Conquer Button
	gameMatrix.identity();
	gameMatrix.Translate(0.0f, 8.0f, 0.0f);
	program->setModelMatrix(gameMatrix);

	float conqbtn_vertices[] = { -6.0f, -2.8f, 6.0f, 6.8f, -6.0f, 6.8f, 6.0f, 6.8f, -6.0f, -2.8f, 6.0f, -2.8f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, conqbtn_vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, playID);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
	
	//About Button
	gameMatrix.identity();
	gameMatrix.Translate(0.0f, -3.0f, 0.0f);
	program->setModelMatrix(gameMatrix);

	float aboutbtn_vertices[] = { -6.0f, 0.5f, 6.0f, 9.5f, -6.0f, 9.5f, 6.0f, 9.5f, -6.0f, 0.5f, 6.0f, 0.5f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, aboutbtn_vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, aboutID);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
	
	//Exit Button
	gameMatrix.identity();
	gameMatrix.Translate(-0.05f, -11.0f, 0.0f);
	program->setModelMatrix(gameMatrix);

	float exitbtn_vertices[] = { -6.0f, 0.5f, 6.0f, 9.5f, -6.0f, 9.5f, 6.0f, 9.5f, -6.0f, 0.5f, 6.0f, 0.5f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, exitbtn_vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, exitID);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
	
}

void Game::LoadStageMap() {
	gameMatrix.identity();
	program->setModelMatrix(gameMatrix);

	//Texture Coords
	float texCoords[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	//Background Image for Map Levels		
	float map_vertices[] = { -20.0f, -30.0f, 20.0f, 30.0f, -20.0f, 30.0f, 20.0f, 30.0f, -20.0f, -30.0f, 20.0f, -30.0f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, map_vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, mapLevelsID);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
	
	SDL_GL_SwapWindow(displayWindow);
}

void Game::LoadMap(std::string filename) {
	std::ifstream infile(filename);
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Player]" || line == "[Weapons]" || line == "[Items]" || line == "[Enemies]") {
			readEntityData(infile);
		}
	}

	infile.close();
}

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
	else {
		bgLevelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			bgLevelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool Game::readLayerData(std::ifstream &stream) {
	while (getline(stream, line)) {
		if (line == "") { break; }
		//For the type of tilelayer
		std::istringstream typeStream(line);
		getline(typeStream, type, '=');
		getline(typeStream, type_value);
		//For the data of tilelayer
		getline(stream, line2);
		std::istringstream dataStream(line);
		getline(dataStream, data, '=');
		getline(dataStream, data_value);
		if (type_value == "Background") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);	
				std::istringstream lineStream(line);
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						bgLevelData[y][x] = val - 1;
					}
					else {
						bgLevelData[y][x] = 79;
					}
				}
			}
		}
	}
	return true;
}

bool Game::readEntityData(std::ifstream &stream) {
	while (getline(stream, line)) {
		if (line == "") { break; }
		//Determine Object type
		std::istringstream sStream(line);
		getline(sStream, type, '=');
		getline(sStream, type_value);
		//If object is player type
		if (type_value == "PLAYER") {
			//Parse next segment
			getline(stream, line);
			std::istringstream sStream(line);
			getline(sStream, type, '=');
			getline(sStream, loc_value);
			//Get location of object
			if (type == "location") {
				std::istringstream lineStream(loc_value);
				getline(lineStream, xPosition, ',');
				getline(lineStream, yPosition, ',');
				float placeX = (float)atoi(xPosition.c_str()) * TILE_SIZE * 2.0f;
				float placeY = (float)atoi(yPosition.c_str()) * -TILE_SIZE * 1.0f;
				placeEntity(placeX, placeY, 1.0f, PLAYER, pStandingSprite);
			}
		}
	}
	return true;
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
	if (selectedLevel == FOREST) {
		glBindTexture(GL_TEXTURE_2D, forestSprites);
	}
	else if (selectedLevel == CANDYLAND) {
		glBindTexture(GL_TEXTURE_2D, candylandSprites);
	}
	else if (selectedLevel == SNOW_TUNDRA) {
		glBindTexture(GL_TEXTURE_2D, snowtundraSprites);
	}
	glDrawArrays(GL_TRIANGLES, 0, 6 * 140 * 40);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	//Draw Player
	player->Draw(program, gameMatrix, player->getMainSprite());

	//Draw Solid Tiles

	//Draw Items

	SDL_GL_SwapWindow(displayWindow);
}

void Game::RenderBackground() {
	//Draw Map
	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			//Draw Background 
			u = (float)(((int)bgLevelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
			v = (float)(((int)bgLevelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;

			spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
			spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
			vertexData.insert(vertexData.end(), {
				TILE_SIZE * x - 0.0325f, -TILE_SIZE * y + 0.0325f,
				TILE_SIZE * x - 0.0325f, (-TILE_SIZE * y) - TILE_SIZE - 0.0325f,
				(TILE_SIZE * x) + 0.0325f + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE - 0.0325f,
				TILE_SIZE * x - 0.0325f, -TILE_SIZE * y + 0.0325f,
				(TILE_SIZE * x + 0.0325f) + TILE_SIZE, (-TILE_SIZE * y) - 0.0325f - TILE_SIZE,
				(TILE_SIZE * x + 0.0325f) + TILE_SIZE, -TILE_SIZE * y + 0.0325f
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

void Game::centerMap() {
	viewMatrix.identity();
	viewMatrix.Translate(-1 * player->getXPos(), -1 * player->getYPos() - 1.5f, 0.0f);
	program->setViewMatrix(viewMatrix);
}

void Game::placeEntity(float startingXPos, float startingYPos, float startingDir, TYPE eType, SheetSprite& eSprite) {
	if (eType == PLAYER) {
		player = new Entity(startingXPos, startingYPos, startingDir, eType, eSprite);
	}
	else if (eType == T_BLOCK)
	{

	}
}

void Game::LoadForestMap() {
	gameMatrix.identity();
	program->setModelMatrix(gameMatrix);
	projectionMatrix.setOrthoProjection(-4.5f, 4.5f, -6.0f, 6.0f, -1.0f, 1.0f);
	program->setProjectionMatrix(projectionMatrix);
	glClearColor(0.54902f, 0.847059f, 1.0f, 1.0f);	//Forest
	glClear(GL_COLOR_BUFFER_BIT);
	LoadMap("maps/Forest/forest_map.txt");
	RenderBackground();
}

void Game::LoadCandylandMap() {
	gameMatrix.identity();
	program->setModelMatrix(gameMatrix);

}
void Game::LoadSnowTundraMap() {
	gameMatrix.identity();
	program->setModelMatrix(gameMatrix);

}
void Game::ProcessEvents(float elapsed) {
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			//Exit Game
			done = true;
		}
		if (state == TITLESCREEN) {
			//Select Option
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				//Keep track of mouse coordinates and convert to game pixels	
				unitX = (((float)event.motion.x / 640.0f) * 3.554f) - 1.777f;
				unitY = (((float)(360 - event.motion.y) / 360.0f) * 2.0f) - 1.0f;

				//If left click
				if (event.button.button == 1) {
					//Play Button (Conquer)
					if (unitX > -0.5f && unitX < 0.5 && unitY > 0.20f && unitY < 0.45f) {
						Mix_PlayChannel(-1, select_sound, 0);
						state = ALL_MAP_LEVELS;
					}
					//About Button
					if (unitX > -0.5f && unitX < 0.5 && unitY > -0.06f && unitY < 0.19f) {
						Mix_PlayChannel(-1, select_sound, 0);
						//state = ABOUT;
					}
					//Exit Button
					if (unitX > -0.5f && unitX < 0.5 && unitY > -0.32f && unitY < -0.07f) {
						Mix_PlayChannel(-1, select_sound, 0);
						done = true;
					}
				}
			}
		}
		else if (state == ALL_MAP_LEVELS) {
			//Hover effect hopefully
			if (event.type == SDL_MOUSEMOTION) {

			}
			//Select level
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				//Keep track of mouse coordinates and convert to game pixels	
				unitX = (((float)event.motion.x / 640.0f) * 3.554f) - 1.777f;
				unitY = (((float)(360 - event.motion.y) / 360.0f) * 2.0f) - 1.0f;

				//If left click
				if (event.button.button == 1) {
					//Forest Map
					if (unitX > -2.0f && unitX < -1.0f && unitY > -1.0f && unitY < -0.6f) {
						Mix_PlayChannel(-1, select_sound, 0);
						selectedLevel = FOREST;
						state = IN_GAME;
						LoadForestMap();

					}
					//Candyland Map
					else if (unitX > -1.7f && unitX < -0.6f && unitY > 0.3f && unitY < 1.0f) {
						if (CANDYLAND > furthestLevel) {
							Mix_PlayChannel(-1, error_sound, 2);
						}
						else {
							Mix_PlayChannel(-1, select_sound, 0);
							selectedLevel = CANDYLAND;
							//LoadCandylandMap();
						}
					}
					//Snow Tundra Map
					else if (unitX > -0.55 && unitX < 0.5f && unitY > -0.35f && unitY < 0.25f) {
						if (SNOW_TUNDRA > furthestLevel) {
							Mix_PlayChannel(-1, error_sound, 2);
						}
						else {
							Mix_PlayChannel(-1, error_sound, 2);
							selectedLevel = SNOW_TUNDRA;
							//LoadSnowTundraMap();
						}
					}
					//Lava Canyon Map
					else if (unitX > 0.55f && unitX < 1.55f && unitY > -0.88f && unitY < -0.3f) {
						Mix_PlayChannel(-1, error_sound, 2);
					}
					//Castle Map
					else if (unitX > 0.6f && unitX < 1.7f && unitY > 0.2f && unitY < 0.93f) {
						Mix_PlayChannel(-1, error_sound, 2);
					}
				}
			}
		}
		else if (state == IN_GAME) {
			if (selectedLevel == FOREST || selectedLevel == CANDYLAND) {
				//Move left
				if (keys[SDL_SCANCODE_LEFT]) {
					//if (player->getXPos() + (-1.0 * player->getXVel() * elapsed) > -6.5f) {
						player->xTranslate(-3.0f * player->getXVel() * elapsed);
					//}
				}
				//Move Right
				if (keys[SDL_SCANCODE_RIGHT]) {
				//	if (player->getXPos() + (player->getXVel() * elapsed) < 8.0f) {
						player->xTranslate(3.0f * player->getXVel() * elapsed);
					//}
				}
			}
			else if (selectedLevel == SNOW_TUNDRA) {
				//Slippery effect
				if (keys[SDL_SCANCODE_LEFT]) {
					/*if (player->getDirection() > 0) {
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
						}*/
				}
			}
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
	switch (state) {
		case TITLESCREEN:
		{
			LoadTitleScreen();
			break;
		}
		case ALL_MAP_LEVELS: {
			LoadStageMap();
			break;
		}
		case IN_GAME: {
			RenderGame();
			break;
		}
		case ABOUT: {
			break;
		}
		case PAUSE: {
			break;
		}
	}
}

void Game::Update(float elapsed) {
	//UpdateHUD();
	if (state == IN_GAME) {
		centerMap();

	}
	
}

void Game::LoadAllTexturesandSound() {
	//Images
	fontSprites = LoadTextureRGBA("images/font1.png");
	bgID = LoadTextureRGBA("images/titlescreenbg.png");
	playID = LoadTextureRGBA("images/conquerbutton.png");
	aboutID = LoadTextureRGBA("images/aboutbutton.png");
	exitID = LoadTextureRGBA("images/exitbutton.png");
	mapLevelsID = LoadTextureRGBA("images/maplevels.png");
	forestSprites = LoadTextureRGBA("maps/Forest/sprites.png");
	candylandSprites = LoadTextureRGBA("maps/Forest/sprites.png");
	snowtundraSprites = LoadTextureRGBA("maps/Forest/sprites.png");
	playerSprites = LoadTextureRGBA("assets/Player/p1_spritesheet.png");

	//Sprites
	pStandingSprite = SheetSprite(playerSprites, 76.0f / 508.0f, 208.0f / 208.0f, 72.0f / 508.0f, 69.0f / 208.0f, 2.0f);


	//Sounds
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	opening_music = Mix_LoadMUS("sounds/opening_music.mp3");
	select_sound = Mix_LoadWAV("sounds/select.wav");
	error_sound = Mix_LoadWAV("sounds/error.wav");
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