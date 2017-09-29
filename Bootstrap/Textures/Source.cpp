#include <Application.h>
#include "TextureApplication.h"

int main()
{
	Application* app = new TextureApplication();
	app->run("Texturing", 1600, 900, false);
	delete(app);
}
