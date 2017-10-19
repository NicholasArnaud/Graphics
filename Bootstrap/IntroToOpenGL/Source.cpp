#include "IntroductionApp.h"

int main()
{
	Application* app = new IntroductionApp();
	app->run("intro to opengl", 1600, 900, false);
	//run is invoked from application
	//--run will call startup from IntroductionApp
	//--run will call update from IntroductionApp
	//--run will call draw from IntroductionApp
	delete app;
}