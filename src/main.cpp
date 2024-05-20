#include "engine.h"
#include "main_scene.h"

int main(int argc, char **argv) {
	bf::Engine e;

	e.changeScene(new bf::MainScene());

	while (e.update());

	return 0;
}