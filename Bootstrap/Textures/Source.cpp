#include <Application.h>
#include "TextureApplication.h"

int main()
{
	Application* app = new TextureApplication();
	app->run("Texturing", 800, 800, false);
	delete(app);
}
