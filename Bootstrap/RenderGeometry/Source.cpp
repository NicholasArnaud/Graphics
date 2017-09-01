#include "Application.h"
#include "GeometryApplication.h"
int main()
{
	Application* app = new GeometryApplication();
	app->run("Rendering", 800, 600, false);
	delete app;
	return 0;
}