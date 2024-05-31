#define SDL_MAIN_HANDLED
#include "server/server_host.h"

int main(int argc, char **argv) {
	bf::ServerHost serverHost(1234);

	return 0;
}