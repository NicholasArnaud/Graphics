
#include "GeometryApplication.h"
int main()
{
	Application * app = new GeometryApplication();
	app->run("nick game", 800, 600, 0);
	delete app;
	return 0;
}