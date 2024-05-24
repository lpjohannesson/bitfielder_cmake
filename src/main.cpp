#include "engine/engine.h"
#include "client/client.h"

int main(int argc, char **argv) {
	bf::Engine engine;
	bf::Client client(engine);

	while (engine.update());

	return 0;
}