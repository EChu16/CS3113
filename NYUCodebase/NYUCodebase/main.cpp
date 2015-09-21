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
GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB,
		GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}

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
		
	ShaderProgram program = ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

	Matrix modelMatrix;
	Matrix viewMatrix;
	Matrix projectionMatrix;
		

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	SDL_Event event;
	bool done = false;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
				
		glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Do all drawing after glClear and before swap window

		program.setModelMatrix(modelMatrix);
		program.setViewMatrix(viewMatrix);
		program.setProjectionMatrix(projectionMatrix);
		
		glUseProgram(program.programID);

		//Clouds
		float cloud_vertices[] = { -3.4f, 1.5f, -1.6f, 1.5f, -1.6f, 1.8f, -1.6f, 1.8f, -3.4f, 1.8f, -3.4f, 1.5f, -1.0f, 1.3f, 0.8f, 1.3f, 0.8f, 1.6f, 0.8f, 1.6f, -1.0f, 1.6f, -1.0f, 1.3f, 1.4f, 1.5f, 3.2f, 1.5f, 3.2f, 1.8f, 3.2f, 1.8f, 1.4f, 1.8f, 1.4f, 1.5f};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, cloud_vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 18);
		glDisableVertexAttribArray(program.positionAttribute);

		//Ground
		float ground_vertices[] = { -3.6f, -2.0f, 3.6f, -2.0f, 3.6f, -1.6f, 3.6f, -1.6f, -3.6f, -1.6f, -3.6f, -2.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ground_vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		
		//House
		float house_vertices[] = { -2.5f, -1.6f, -0.5f, -1.6f, -0.5f, -0.6f, -0.5f, -0.6f, -2.5f, -0.6f, -2.5f, -1.6f, -2.5f, -0.6f, -0.5f, -0.6f, -1.5f, 0.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, house_vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 9);
		glDisableVertexAttribArray(program.positionAttribute);

		//Windmill
		float windmill_vertices[] = { 0.0f, -1.6f, .8f, -1.6f, 0.4f, 0.3f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, windmill_vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(program.positionAttribute);

		GLuint texID = LoadTexture("images/grass.jpg");
		
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
