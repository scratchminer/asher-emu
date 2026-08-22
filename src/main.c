// Asher, a Playdate emulator
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <unicorn/unicorn.h>

#include "device.h"

static SDL_Window *sdlWindow;
static SDL_Surface *sdlSurface;

int main(int argc, char **argv) {
	/*if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		printf("SDL3 initialization failed: %s\n", SDL_GetError());
		return -1;
	}*/
	
	if (argc == 1) {
		printf("No bootloader given!\n");
		return -1;
	}
	
	asher_device *device = asher_device_create(ASHER_DEVICE_DVT1);
	asher_device_load_boot(device, argv[1]);
	asher_device_reset(device);
	
	if (!asher_device_run(device)) {
		return -1;
	}
	
	return 0;
}
