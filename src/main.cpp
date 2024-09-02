#include "game.hpp"
#include "utils.hpp"

#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <string>

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
	if (argc != 2) {
		SDL_Log("Usage: ./Panorama [file]");
		return SDL_APP_FAILURE;
	}

	std::srand(std::time(nullptr));

	SDL_Log("Initializing game\n");

	SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft");

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to init SDL: %s\n", SDL_GetError());
		ERROR_BOX("Failed to initialize SDL, there is something wrong with your system");
		return SDL_APP_FAILURE;
	}

	try {
		*appstate = new Game(argv[1]);
	} catch (std::runtime_error e) {
		SDL_Log("Error: %s", e.what());
	} catch (...) {
		SDL_Log("Uncaught exception");
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, const SDL_Event* event) {
	if (static_cast<Game*>(appstate)->event(*event)) {
		return SDL_APP_FAILURE;
	} else {
		return SDL_APP_CONTINUE;
	}
}

#ifdef DEBUG
#include <filesystem>
#endif

SDL_AppResult SDL_AppIterate(void* appstate) {
	try {
		if (static_cast<Game*>(appstate)->iterate()) {
			return SDL_APP_FAILURE;
		} else {
			return SDL_APP_CONTINUE;
		}
	}
#ifdef DEBUG
	catch (const std::filesystem::filesystem_error& error) {

		SDL_Log("Filesystem Error:");
		SDL_Log("what():  %s", error.what());
		SDL_Log("path1(): %s", error.path1().c_str());
		SDL_Log("path2(): %s", error.path2().c_str());
		SDL_Log("code().value():    %d", error.code().value());
		SDL_Log("code().message():  %s", error.code().message().data());
		SDL_Log("code().category(): %s", error.code().category().name());

		return SDL_APP_CONTINUE;
	}
#endif
	catch (const std::runtime_error& error) {
#ifdef DEBUG
		SDL_Log("Uncaught exception: %s", error.what());

		static_cast<Game*>(appstate)->pause();

		return SDL_APP_CONTINUE;
#else
		ERROR_BOX("Exception thrown, the game might not function correctly");

		return SDL_APP_CONTINUE;
#endif
	}
}

void SDL_AppQuit(void* appstate) {
	delete static_cast<Game*>(appstate);

	SDL_Quit();
}
