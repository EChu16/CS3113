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

	Matrix modelMatrix;
	Matrix viewMatrix;
	Matrix projectionMatrix;
		

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	SDL_Event event;
	bool done = false;
	float angle = 0.0;
	float lastFrameTicks = 0.0;

	while (!done) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		angle += elapsed;

		glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Do all drawing after glClear and before swap 

		program.setModelMatrix(modelMatrix);
		program.setViewMatrix(viewMatrix);
		program.setProjectionMatrix(projectionMatrix);
		
		glUseProgram(program.programID);
		GLuint cloudID = LoadTextureRGBA("images/cloud.png");
		GLuint grassID = LoadTextureRGB("images/grass.jpg");
		GLuint brickID = LoadTextureRGB("images/brickwall.jpg");
		GLuint grayID = LoadTextureRGB("images/gray.jpg");
		GLuint bladeID = LoadTextureRGBA("images/blades.png");

		//Texture Coords
		float texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };

		//Clouds - Cloud 1
		float cloud1_vertices[] = { -3.4f, 1.3f, -1.6f, 1.3f, -1.6f, 1.8f, -1.6f, 1.8f, -3.4f, 1.8f, -3.4f, 1.3f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, cloud1_vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords); 
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, cloudID);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//Cloud 2
		float cloud2_vertices[] = { -1.0f, 1.1f, 0.8f, 1.1f, 0.8f, 1.6f, 0.8f, 1.6f, -1.0f, 1.6f, -1.0f, 1.1f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, cloud2_vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, cloudID);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//Cloud 3
		float cloud3_vertices[] = { 1.4f, 1.3f, 3.2f, 1.3f, 3.2f, 1.8f, 3.2f, 1.8f, 1.4f, 1.8f, 1.4f, 1.3f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, cloud3_vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, cloudID);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//Ground
		float ground_vertices[] = { -3.6f, -2.0f, 3.6f, -2.0f, 3.6f, -1.6f, 3.6f, -1.6f, -3.6f, -1.6f, -3.6f, -2.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ground_vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, grassID);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//House base
		float house_vertices[] = { -2.5f, -1.6f, -0.5f, -1.6f, -0.5f, -0.6f, -0.5f, -0.6f, -2.5f, -0.6f, -2.5f, -1.6f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, house_vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, brickID);

		glDrawArrays(GL_TRIANGLES, 0, 9);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//House roof
		float roof_vertices[] = { -2.5f, -0.6f, -0.5f, -0.6f, -1.5f, 0.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, roof_vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, brickID);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//Windmill
		float windmill_vertices[] = { 0.0f, -1.6f, .8f, -1.6f, 0.4f, 0.3f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, windmill_vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, grayID);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		
		//Windmill Blades
		float blade_vertices[] = { -0.15f, -0.3f, 0.8f, -0.3f, 0.8f, 0.7f, 0.8f, 0.7f, -0.15f, 0.7f, -0.15f, -0.3f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, blade_vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, bladeID);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		/*modelMatrix.identity();
		modelMatrix.Rotate(angle);*/

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
