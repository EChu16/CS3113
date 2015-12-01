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
	projectionMatrix.setOrthoProjection(-20.0f, 20.0f, -30.0f, 30.0f, -1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	program->setModelMatrix(gameMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);

	state = TITLESCREEN;
	furthestLevel = ONE;

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
	Mix_HaltMusic();

	//Texture Coords
	float texCoords[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	//Background Image for Map Levels	
	float bg_vertices[] = { -20.0f, -30.0f, 20.0f, 30.0f, -20.0f, 30.0f, 20.0f, 30.0f, -20.0f, -30.0f, 20.0f, -30.0f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, bg_vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, bgID);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

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

void Game::ProcessEvents(float elapsed) {
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			//Exit Game
			done = true;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			//Keep track of mouse coordinates and convert to game pixels	
			float unitX = (((float)event.motion.x / 640.0f) * 3.554f) - 1.777f;
			float unitY = (((float)(360 - event.motion.y) / 360.0f) * 2.0f) - 1.0f;
			//If left click
			if (event.button.button == 1) {
				//Play Button (Conquer)
				if (unitX > -0.5f && unitX < 0.5 && unitY > 0.20f && unitY < 0.45f) {
					Mix_PlayChannel(-1, select_sound, 0);
					//state = ALL_MAP_LEVELS;
				}
				//About Button
				if (unitX > -0.5f && unitX < 0.5 && unitY > -0.06f && unitY < 0.19f) {
					Mix_PlayChannel(-1, select_sound, 0);
					state = ABOUT;
				}
				//Exit Button
				if (unitX > -0.5f && unitX < 0.5 && unitY > -0.32f && unitY < -0.07f) {
					Mix_PlayChannel(-1, select_sound, 0);
					done = true;
				}
			}
		}
		else if (state == GAMEOVER) {
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
		case ALL_MAP_LEVELS: {
			LoadStageMap();
			break;
		}
		case ABOUT: {
			break;
		}
		case PAUSE: {
			break;
		}
		case GAMEOVER: {
			GameOver();
			break;
		}
	}
}

void Game::Update(float elapsed) {

}

void Game::LoadAllTexturesandSound() {
	//Images
	fontSprites = LoadTextureRGBA("images/font1.png");
	bgID = LoadTextureRGBA("images/titlescreenbg.png");
	playID = LoadTextureRGBA("images/conquerbutton.png");
	aboutID = LoadTextureRGBA("images/aboutbutton.png");
	exitID = LoadTextureRGBA("images/exitbutton.png");

	//Sounds
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	opening_music = Mix_LoadMUS("music/opening_music.mp3");
	select_sound = Mix_LoadWAV("music/select.wav");
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