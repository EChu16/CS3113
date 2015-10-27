/*
Platformer Game
Erich Chu
*/

#include "Game.h"

int main(int argc, char *argv[])
{
	Game platformer;
	while (!platformer.UpdateAndRender()) {}
	SDL_Quit();
	return 0;
}
