#include "LightingApp.h"
#include <Application.h>


int main()
{
	Application* app = new LightingApp();
	app->run("Lighting", 800, 800, false);
	delete(app);
}
