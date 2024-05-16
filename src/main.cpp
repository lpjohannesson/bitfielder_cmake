#include "engine.h"
#include "main_scene.h"

int main(int argc, char **argv) {
	bf::engine.start();
	bf::engine.changeScene(new bf::MainScene());

	while (bf::engine.update());

	bf::engine.end();

	return 0;
}