#include "IntrodutionApp.h"
#include "Camera.h"


int main()
{
	Application* app = new IntrodutionApp();
	app->run("Intro To Opengl", 1600, 900, false);
	delete app;
}
