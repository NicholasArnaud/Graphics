#include "CameraApp.h"
#include <Application.h>

int main()
{
	Application* app = new CameraApp();
	app->run("Rendering", 800, 600, false);
	delete app;
	return 0;
}