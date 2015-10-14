#include "Game.h"

Game::Game() :done(false), lastFrameTicks(0.0f), victory(false) {
	//Create player entity
	player = new Entity(0.0f, -1.7f, 0.3f, 0.2f, 500.0f, PLAYER);

	//Hold enemy position
	float xPosHolder = -1.5f;
	float yPosHolder = 1.5f;
	//Create enemy entities
	for (int i = 0; i < NUM_ALIENS; i++) {
		enemies.push_back(new Entity(xPosHolder, yPosHolder, 0.3, 0.3, 0.5f, ENEMY));
		xPosHolder += 0.7f;
		if (((i + 1) % 5 == 0) && i != 0) {
			xPosHolder = -1.5f;
			yPosHolder -= 0.6f;
		}
	}
	numEnemiesAlive = NUM_ALIENS;
	lastMonsterBullet = 0.0f;
	randEnemy = floor(rand() % NUM_ALIENS);
	Setup();
}

Game::~Game() {
	delete program;
}

void Game::LoadTitleScreen() {
	gameMatrix.identity();
	gameMatrix.Translate(-0.5f, 0.85f, 0.0f);
	program->setModelMatrix(gameMatrix);
	std::string spaceText = "Space";
	DrawText(fontSprites, spaceText, 0.7f, -0.4f);

	gameMatrix.identity();
	gameMatrix.Translate(-0.9f, 0.45f, 0.0f);
	program->setModelMatrix(gameMatrix);
	std::string invadersText = "Invaders";
	DrawText(fontSprites, invadersText, 0.7f, -0.4f);

	gameMatrix.identity();
	gameMatrix.Translate(-1.5f, -1.0f, 0.0f);
	program->setModelMatrix(gameMatrix);
	std::string instructionsText = "Press Enter to start";
	DrawText(fontSprites, instructionsText, 0.48f, -0.3);
}

void Game::LoadGamePlay() {
	//Draw Player
	player->Draw(program, gameMatrix, playerImg);

	//Draw Alien
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->checkAlive() == true) {
			enemies[i]->Draw(program, gameMatrix, monsterImg);
		}
	}

	//Draw Bullets
	for (int i = 0; i < bullets.size(); i++) {
		if(bullets[i]->isAlive())
			bullets[i]->Draw(program, gameMatrix, bulletImg);
	}
	SDL_GL_SwapWindow(displayWindow);

}

void Game::GameOver() {
	gameMatrix.identity();
	gameMatrix.Translate(-1.4f, 0.85f, 0.0f);
	program->setModelMatrix(gameMatrix);
	std::string gameOver = "Game Over";
	DrawText(fontSprites, gameOver, 0.7f, -0.35f);

	if (victory == true) {
		gameMatrix.identity();
		gameMatrix.Translate(-0.9f, 0.35f, 0.0f);
		program->setModelMatrix(gameMatrix);
		std::string winText = "You Win";
		DrawText(fontSprites, winText, 0.7f, -0.4f);
	}
	else {
		gameMatrix.identity();
		gameMatrix.Translate(-1.0f, 0.35f, 0.0f);
		program->setModelMatrix(gameMatrix);
		std::string loseText = "You Lose";
		DrawText(fontSprites, loseText, 0.7f, -0.4f);
	}

	gameMatrix.identity();
	gameMatrix.Translate(-1.5f, -1.0f, 0.0f);
	program->setModelMatrix(gameMatrix);
	std::string goodbyeText = "Press Enter to close";
	DrawText(fontSprites, goodbyeText, 0.48f, -0.3);
	SDL_GL_SwapWindow(displayWindow);
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
	projectionMatrix.setOrthoProjection(-3.0f, 3.0f, -2.0f, 2.0f, -1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	program->setModelMatrix(gameMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);

	state = TITLESCREEN;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	LoadAllTextures();
	Render();

	SDL_GL_SwapWindow(displayWindow);
}

void Game::ProcessEvents(float elapsed) {
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if (state == TITLESCREEN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				state = GAMEPLAY;

			}
		}
		if (state == GAMEPLAY){
			if (event.type == SDL_KEYDOWN) {
				//Player Controls
				//Move Left
				if (keys[SDL_SCANCODE_LEFT]) {
					if (player->getxPos() + (-1.0 * player->getVelocity() * elapsed) > -3.0f) {
						player->xTranslate(-1.0 * player->getVelocity() * elapsed);
					}
				}
				//Move Right
				else if (keys[SDL_SCANCODE_RIGHT]) {
					if (player->getxPos() + (player->getVelocity() * elapsed) < 3.0f) {
						player->xTranslate(player->getVelocity() * elapsed);
					}
				}
				//Shoot bullet
				else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					player->shootBullet(bullets);
				}
			}
		}
		if (state == GAMEOVER) {
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				done = true;
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
	case GAMEPLAY: {
		LoadGamePlay();
		break;
	}
	case GAMEOVER: {
		GameOver();
		break;
	}
	}
}

void Game::Update(float elapsed) {
	if (state == GAMEPLAY) {
		if (gameEnded() == true) {
			state = GAMEOVER;
		}
		//Aliens move
		else if (enemies[0]->willCollide(0.1f) || enemies[4]->willCollide(0.1f)) {
			for (int i = 0; i < enemies.size(); i++) {
				enemies[i]->changeDirection();
				enemies[i]->xTranslate(enemies[i]->getDirection() * enemies[i]->getVelocity() * elapsed + (enemies[i]->getDirection() * 0.1));
				enemies[i]->yTranslate(-0.05f);

				if (player->collidesWith(enemies[i])) {
					state = GAMEOVER;
					break;
				}
			}
		}
		else {
			for (int i = 0; i < enemies.size(); i++) {
				enemies[i]->xTranslate(enemies[i]->getDirection() * enemies[i]->getVelocity() * elapsed);
				if (player->collidesWith(enemies[i])) {
					state = GAMEOVER;
					break;
				}
			}
		}
		//Shoot Monster Bullets
		lastMonsterBullet += elapsed;
		if (lastMonsterBullet > 3.0f) {
			lastMonsterBullet = 0.0f;
			while (!enemies[randEnemy]->checkAlive()) {
				randEnemy = floor(rand() % NUM_ALIENS);
			}
			enemies[randEnemy]->shootBullet(bullets);
		}

		//Move Bullets
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i]->isAlive()) {
				bullets[i]->yTranslate(bullets[i]->getVelocity() * elapsed);
				if (bullets[i]->getyPos() > 2.2f || bullets[i]->getyPos() < -2.2f) {
					bullets[i]->dies();
				}
				for (int a = 0; a < enemies.size(); a++) {
					if (enemies[a]->checkAlive() == true && bullets[i]->collidesWith(enemies[a])) {
						enemies[a]->dies();
						bullets[i]->dies();
						--numEnemiesAlive;
						if (numEnemiesAlive == 0)
							victory = true;
						break;
					}
				}
			}
			if (bullets[i]->collidesWith(player)) {
				player->dies();
			}
			
		}
	}
}

bool Game::gameEnded() const {
	return numEnemiesAlive == 0 || !player->checkAlive();
}

void Game::LoadAllTextures() {
	spaceInvaderSprites = LoadTextureRGBA("images/sprites.png");
	fontSprites = LoadTextureRGBA("images/font1.png");
	playerImg = SheetSprite(spaceInvaderSprites, 0.0f / 256.0f, 72.0f / 256.0f, 85.0f / 256.0f, 57.0f / 256.0f, 0.3f);
	monsterImg = SheetSprite(spaceInvaderSprites, 0.0f / 256.0f, 0.0f / 256.0f, 92.0f / 256.0f, 70.0f / 256.0f, 0.3f);
	bulletImg = SheetSprite(spaceInvaderSprites, 0.0f / 256.0f, 131.0f / 256.0f, 50.0f / 256.0f, 86.0f / 256.0f, 0.1f);
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