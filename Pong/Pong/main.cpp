#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
GLuint LoadTextureRGB(const char *image_path) {
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

GLuint LoadTextureRGBA(const char *image_path) {
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

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	//Setup Code
	glViewport(0, 0, 640, 360);
		
	ShaderProgram program = ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	Matrix paddle1Matrix;
	Matrix paddle2Matrix;
	Matrix ballMatrix;
	Matrix viewMatrix;
	Matrix projectionMatrix;
		
	GLuint whiteID = LoadTextureRGB("images/white.jpg");

	projectionMatrix.setOrthoProjection(-3.5f, 3.5f, -2.0f, 2.0f, -1.0f, 1.0f);
	SDL_Event event;
	bool done = false;
	bool gameStarted = false;
	int player1Score = 0;
	int player2Score = 0;

	//Pong positions
	//Paddle 1
	float paddle1posX1 = -3.1f;
	float paddle1posX2 = -3.5f;
	float paddle1posY1 = 0.8f;
	float paddle1posY2 = -0.8f;
	
	//Paddle 2
	float paddle2posX1 = 3.1f;
	float paddle2posX2 = 3.5f;
	float paddle2posY1 = 0.8f;
	float paddle2posY2 = -0.8f;

	//Ball
	float ballposX = 0.0f;
	float ballposY = 0.0f;
	float ballSpeed = 0.001f;
	float ballDirectionX = 1.0f;
	float ballDirectionY = 1.0f;

	while (!done) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Controls for the game
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			if (event.type == SDL_KEYDOWN) {
				//Player 1 Controls
				if (event.key.keysym.scancode == SDL_SCANCODE_W) {
					if (paddle1posY1 < 2.0f){
						paddle1posY1 += 0.2f;
						paddle1posY2 += 0.2f;
						paddle1Matrix.Translate(0.0f, 0.2f, 0.0f);
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					if (paddle1posY2 > -2.0f){
						paddle1posY1 -= 0.2f;
						paddle1posY2 -= 0.2f;
						paddle1Matrix.Translate(0.0f, -0.2f, 0.0f);
					}
				}
				//Player 2 Controls
				if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					if (paddle2posY1 < 2.0f){
						paddle2posY1 += 0.2f;
						paddle2posY2 += 0.2f;
						paddle2Matrix.Translate(0.0f, 0.2f, 0.0f);
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					if (paddle2posY2 > -2.0f){
						paddle2posY1 -= 0.2f;
						paddle2posY2 -= 0.2f;
						paddle2Matrix.Translate(0.0f, -0.2f, 0.0f);
					}
				}
				//Start the game
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					if (gameStarted == false) {
						gameStarted = true;
					}
				}
			}
		}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Do all drawing after glClear and before swap 

		program.setModelMatrix(paddle1Matrix);
		program.setViewMatrix(viewMatrix);
		program.setProjectionMatrix(projectionMatrix);
		
		glUseProgram(program.programID);

		//Texture Coords
		float texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };

		//Paddle 1
		float paddle1[] = { -3.5f, -0.8f, -3.1f, -0.8f, -3.1f, 0.8f, -3.1f, 0.8f, -3.5f, 0.8f, -3.5f, -0.8f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle1);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, whiteID);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		program.setModelMatrix(paddle2Matrix);
		//Paddle 2
		float paddle2[] = { 3.1f, -0.8f, 3.5f, -0.8f, 3.5f, 0.8f, 3.5f, 0.8f, 3.1f, 0.8f, 3.1f, -0.8f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle2);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, whiteID);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		program.setModelMatrix(ballMatrix);
		//Ball
		float ball[] = { -0.2f, -0.2f, 0.2f, -0.2f, 0.2f, 0.2f, 0.2f, 0.2f, -0.2f, 0.2f, -0.2f, -0.2f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, whiteID);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		if (gameStarted == true) {
			//If player 2 wins
			if ((ballposX - 0.2f) <= paddle1posX2) {
				gameStarted = false;
				player2Score += 1;
				ballMatrix.Translate(-ballposX, -ballposY, 0.0f);
				ballposX = 0.0f;
				ballposY = 0.0f;
				ballDirectionX = 1.0f;
				ballDirectionY = 1.0f;
			}
			//If player 1 wins
			else if ((ballposX + 0.2f) >= paddle2posX2) {
				gameStarted = false;
				player1Score += 1;
				ballMatrix.Translate(-ballposX, -ballposY, 0.0f);
				ballposX = 0.0f;
				ballposY = 0.0f;
				ballDirectionX = 1.0f;
				ballDirectionY = 1.0f;
			}
			//If ball bounces off top wall
			else  if (ballposY + 0.2f >= 2.0f) {
				ballDirectionY = -ballDirectionY;
				ballposX += ballSpeed * ballDirectionX;
				ballposY += (ballSpeed * ballDirectionY);
				ballMatrix.Translate((ballSpeed * ballDirectionX), (ballSpeed * ballDirectionY), 0.0f);
			}
			//If ball bounces off bottom wall
			else if (ballposY - 0.2f <= -2.0f) {
				ballDirectionY = -ballDirectionY;
				ballposX -= (ballSpeed * ballDirectionX);
				ballposY += (ballSpeed * ballDirectionY);
				ballMatrix.Translate((ballSpeed * ballDirectionX), (ballSpeed * ballDirectionY), 0.0f);
			}
			//If ball hits first paddle
			else if ((ballposX - 0.2f) <= paddle1posX1 && (ballposY - 0.2f) <= paddle1posY1 && (ballposY + 0.2f) >= paddle1posY2) {
				ballDirectionX = -ballDirectionX;
				ballposX += (ballSpeed * ballDirectionX);
				ballposY += (ballSpeed * ballDirectionY);
				ballMatrix.Translate((ballSpeed * ballDirectionX), (ballSpeed * ballDirectionY), 0.0f);
			}
			//If ball hits second paddle
			else if ((ballposX + 0.2f) >= paddle2posX1 && (ballposY - 0.2f) <= paddle2posY1 && (ballposY + 0.2f) >= paddle2posY2) {
				ballDirectionX = -ballDirectionX;
				ballposX += (ballSpeed * ballDirectionX);
				ballposY += (ballSpeed * ballDirectionY);
				ballMatrix.Translate((ballSpeed * ballDirectionX), (ballSpeed * ballDirectionY), 0.0f);
			}
			//If ball is moving without collision
			else {
				ballposX += (ballSpeed * ballDirectionX);
				ballposY += (ballSpeed * ballDirectionY);
				ballMatrix.Translate((ballSpeed * ballDirectionX), (ballSpeed * ballDirectionY), 0.0f);
			}
		}
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
