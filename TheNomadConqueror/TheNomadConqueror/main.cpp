/*
The Nomad Conqueror
Erich Chu
*/

#include "Game.h"

int main(int argc, char *argv[])
{
	Game theNomadConqueror;
	while (!theNomadConqueror.UpdateAndRender()) {}
	SDL_Quit();
	return 0;
}
