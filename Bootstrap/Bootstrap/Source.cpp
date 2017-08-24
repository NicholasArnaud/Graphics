#include "IntrodutionApp.h"
#include "CameraApp.h"


int main()
{
	Application* app = new CameraApp();
	app->run("Intro To Opengl", 1600, 900, false);
	delete app;
}
