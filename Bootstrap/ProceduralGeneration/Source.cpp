#include "Application.h"
#include "ProceduralGenerationApp.h"

int main()
{
	Application* app = new ProceduralGenerationApp();
	app->run("Procedural Generation", 800, 600, false);
	delete app;
	return 0;
}
