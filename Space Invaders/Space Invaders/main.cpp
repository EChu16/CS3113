/*
	Space Invaders
	Erich Chu
*/

#include "Game.h"

int main(int argc, char *argv[])
{
	Game spaceInvaders;
	while (!spaceInvaders.UpdateAndRender()) {}
	SDL_Quit();
	return 0;
}
