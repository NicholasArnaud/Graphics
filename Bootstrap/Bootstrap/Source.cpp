#include "IntrodutionApp.h"
int main()
{
	Application* app = new IntrodutionApp();
	app->run("intro to opengl", 1600, 900, false);
	delete app;
}