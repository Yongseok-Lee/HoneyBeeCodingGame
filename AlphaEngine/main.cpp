#include "GameApp.h"

int main(int argc, char* argv[])
{
	GameApp* game = GameApp::GetInstance();
	return game->GameLoop();
}
