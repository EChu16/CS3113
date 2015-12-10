/*
The Nomad Conqueror
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
		else if (line == "[Player]" || line == "[Items]" || line == "[Weapons]" || line == "[Enemies]") {
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
		if (line == "") { getline(stream, line); }
		//Determine Object type
		std::istringstream sStream(line);
		getline(sStream, type, '=');
		getline(sStream, type_value);
		//If object is player type
		if (type_value == "PLAYER" || type_value == "GOLD_KEY" || type_value == "COIN" || type_value == "TINY_RAY_GUN" || type_value == "SPIKE" || 
			type_value == "SPIDER" || type_value == "GHOST" || type_value == "FISH") {
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
				float placeX = (float)atoi(xPosition.c_str()) * TILE_SIZE * 1.0f;
				float placeY = (float)atoi(yPosition.c_str()) * -TILE_SIZE * 1.0f;
				if (type_value == "PLAYER") {
					player = new Entity(placeX, placeY, 1.0f, PLAYER);
					player->setMainSprite(pStandingSprite);
				}
				else if (type_value == "GOLD_KEY") {
					goldkey = new Entity(placeX + 0.5f, placeY + 0.5f, 1.0f, GOLD_KEY);
					goldkey->setMainSprite(goldkeySprite);
				}
				else if (type_value == "COIN") {
					coins.push_back(new Entity(placeX + 0.5f, placeY + 0.5f, 1.0f, COIN));
					coins.back()->setMainSprite(coinSprite);
				}
				else if (type_value == "TINY_RAY_GUN") {
					tiny_ray_gun = new Entity(placeX + 0.5f, placeY + 0.5f, 1.0f, TINY_RAY_GUN);
					tiny_ray_gun->setMainSprite(tRayGunSprite);
				}
				else if (type_value == "SPIKE") {
					spikes.push_back(new Entity(placeX + 0.5f, placeY + 0.5f, 1.0f, SPIKE));
					spikes.back()->setMainSprite(spikeSprite);
				}
				else if (type_value == "SPIDER") {
					enemies.push_back(new Entity(placeX + 0.5f, placeY + 0.5f, 1.0f, SPIDER));
					enemies.back()->setMainSprite(spiderSprite);
				}
				else if (type_value == "GHOST") {
					enemies.push_back(new Entity(placeX + 0.5f, placeY + 0.5f, 1.0f, GHOST));
					enemies.back()->setMainSprite(ghostSprite);
				}
				else if (type_value == "FISH") {
					enemies.push_back(new Entity(placeX + 0.5f, placeY + 0.5f, 1.0f, FISH));
					enemies.back()->setMainSprite(fishSprite);
				}
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
	glDrawArrays(GL_TRIANGLES, 0, vertexData.size()/2);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	//Draw Player
	player->Draw(program, gameMatrix, player->getMainSprite());

	//Draw Key
	if (goldkey->isAlive()) { goldkey->Draw(program, gameMatrix, goldkey->getMainSprite()); }
	
	//Draw Ray Gun
	if (tiny_ray_gun->isAlive()) { tiny_ray_gun->Draw(program, gameMatrix, tiny_ray_gun->getMainSprite()); }

	//Draw Coins
	for (int i = 0; i < coins.size(); i++) {
		if (coins[i]->isAlive()) {
			coins[i]->Draw(program, gameMatrix, coins[i]->getMainSprite());
		}
	}

	//Draw Spikes
	for (int i = 0; i < spikes.size(); i++) {
		spikes[i]->Draw(program, gameMatrix, spikes[i]->getMainSprite());
	}

	//Draw Enemies
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Draw(program, gameMatrix, enemies[i]->getMainSprite());
	}

	SDL_GL_SwapWindow(displayWindow);
}

void Game::RenderBackground() {
	//Draw Map
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (bgLevelData[y][x] != 79) {
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
				for (int a = 0; a < solids.size(); a++) {
					if (solids[a] == (int)bgLevelData[y][x]) {
						if (solids[a] == 32 || solids[a] == 42 || solids[a] == 52) {
							solidTiles.push_back(new Entity(TILE_SIZE * x, -TILE_SIZE * y - (TILE_SIZE / 4.0f), 1.0f, HALF_BLOCK));
						}
						else{
							solidTiles.push_back(new Entity(TILE_SIZE * x, -TILE_SIZE * y - (TILE_SIZE / 2.0f), 1.0f, BLOCK));
						}
						
					}
				}
			}
		}
	}
}

void Game::centerMap() {
	viewMatrix.identity();
	viewMatrix.Translate(-1 * player->getXPos(), -1 * player->getYPos() - 1.5f, 0.0f);
	program->setViewMatrix(viewMatrix);
}

void Game::LoadForestMap() {
	gameMatrix.identity();
	program->setModelMatrix(gameMatrix);
	projectionMatrix.setOrthoProjection(-4.5f, 4.5f, -6.0f, 6.0f, -1.0f, 1.0f);
	program->setProjectionMatrix(projectionMatrix);
	glClearColor(0.54902f, 0.847059f, 1.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT);
	solids = {2, 3, 5, 15, 32, 33, 42, 47, 57, 52, 67, 71, 77, 82};
	LoadMap("maps/Forest/testmap.txt");
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
	if (state == IN_GAME) {
		if (selectedLevel == FOREST || selectedLevel == CANDYLAND) {
			//Move left
			if (keys[SDL_SCANCODE_LEFT]) {
				player->changeDirection(-1.0f);
				if (player->getXPos() - (player->getXVel() * elapsed) < 0.335f) {
					player->changeStatic(true);
					player->changeXVel(0.0f);
				}
				else if (player->isOnSurface()) {
					player->changeStatic(false);
					player->changeXVel(-1 * 6.0f);
				}
				else {
					player->changeStatic(false);
					player->changeXVel(-1 * 4.0f);
				}
			}
			//Move Right
			else if (keys[SDL_SCANCODE_RIGHT]) {
				player->changeDirection(1.0f);
				if (player->getXPos() + (player->getXVel() * elapsed) > 143.5f && !player->hasGoldKey()) {
					player->changeStatic(true);
					player->changeXVel(0.0f);
				}
				else if (player->getXPos() + (player->getXVel() * elapsed) > 148.5f) {
					player->changeStatic(true);
					player->changeXVel(0.0f);
				}
				else if (player->isOnSurface()) {
					player->changeStatic(false);
					player->changeXVel(6.0f);
				}
				else {
					player->changeStatic(false);
					player->changeXVel(4.0f);
				}
			}
			else {
				player->changeStatic(true);
				player->changeXVel(0.0f);
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
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					if (player->isOnSurface()) {
						player->changeOnSurface(false);
						player->changeYVel(10.0f);
					}
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
		//Check Player Collision
		//Tiles
		for (int i = 0; i < solidTiles.size(); i++) {
			if (player->collidesWith(solidTiles[i])) {
				player->collidedAction(solidTiles[i]);
			}
		}
		//Items
		for (int i = 0; i < coins.size(); i++) {
			if (player->collidesWith(coins[i])) {
				player->collidedAction(coins[i]);
			}
		}
		//Keys
		if (player->collidesWith(goldkey)) { player->collidedAction(goldkey); }
		//Weapons
		if (player->collidesWith(tiny_ray_gun)) { player->collidedAction(tiny_ray_gun); }
		//Enemies
		for (int i = 0; i < enemies.size(); i++) {
			if (player->collidesWith(enemies[i])) {
				player->collidedAction(enemies[i]);
			}
		}
		//Spikes
		for (int i = 0; i < spikes.size(); i++) {
			if (player->collidesWith(spikes[i])) {
				player->collidedAction(spikes[i]);
			}
		}
		//Update Player values
		player->Update(elapsed);
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
	itemSprites = LoadTextureRGBA("assets/Items/sprites.png");
	weaponSprites = LoadTextureRGBA("assets/Weapons/sprites.png");
	hudSprites = LoadTextureRGBA("assets/HUD/sprites.png");

	//Sprites
	pStandingSprite = SheetSprite(playerSprites, 76.0f / 508.0f, 208.0f / 208.0f, 72.0f / 508.0f, 69.0f / 208.0f, 2.0f);
	coinSprite = SheetSprite(itemSprites, 0.0f / 216.0f, 144.0f / 216.0f, 70.0f / 216.0f, 70.0f / 216.0f, 1.0f);
	goldkeySprite = SheetSprite(itemSprites, 0.0f / 216.0f, 72.0f / 216.0f, 70.0f / 216.0f, 70.0f / 216.0f, 1.0f);
	spikeSprite = SheetSprite(itemSprites, 0.0f / 216.0f, 0.0f / 216.0f, 70.0f / 216.0f, 70.0f / 216.0f, 1.0f);
	tRayGunSprite = SheetSprite(weaponSprites, 0.0f / 144.0f, 0.0f / 144.0f, 70.0f / 144.0f, 70.0f / 144.0f, 1.2f);
	spiderSprite = SheetSprite(forestSprites, 432.0f / 720.0f, 360.0f / 720.0f, 70.0f / 720.0f, 70.0f / 720.0f, 1.2f);
	ghostSprite = SheetSprite(forestSprites, 216.0f / 720.0f, 288.0f / 720.0f, 70.0f / 720.0f, 70.0f / 720.0f, 1.2f);
	fishSprite = SheetSprite(forestSprites, 576.0f / 720.0f, 174.0f / 720.0f, 70.0f / 720.0f, 70.0f / 720.0f, 1.0);


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