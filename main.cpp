#include <application.h>

constexpr auto WIDTH = 800;
constexpr auto HEIGHT = 600;
constexpr auto TITLE = "OpenGL Learning";

int main(int argc, char* argv[]) {
	Application app(WIDTH, HEIGHT, TITLE);
	app.run();
	return 0;
}



