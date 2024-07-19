#include "managers/glManager.hpp"

#include "third_party/glad/glad.h"
#include "utils.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

void checkGLError(const char* stmt, const char* fname, int line) {
	GLenum err = glGetError();
	while (err != GL_NO_ERROR) {
		SDL_Log("OpenGL error %d at %s:%d - for %s\n", err, fname, line, stmt);
		err = glGetError(); // To catch multiple errors
	}
}

GLManager::GLManager(SDL_Window* window) {
#ifdef GLES
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
#ifdef DEBUG
												  | SDL_GL_CONTEXT_DEBUG_FLAG
#endif
	);
#endif
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);

	mContext = SDL_GL_CreateContext(window);
	if (window == nullptr) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to create window: %s\n", SDL_GetError());
		ERROR_BOX("Failed to initialize OpenGL Context, there is something "
				  "wrong with your OpenGL");
		throw 1;
	}

#ifdef GLES
	if (gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0) {
#else
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0) {
#endif
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init glad!\n");
		ERROR_BOX("Failed to initialize GLAD, there is something wrong with your OpenGL");

		throw 1;
	}

	// Set VSync
	SDL_GL_SetSwapInterval(1);

#ifdef DEBUG
	/*
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	*/
#endif

	SDL_GL_MakeCurrent(window, mContext);

	glEnable(GL_DEPTH_TEST);
	// glDepthMask(GL_FALSE);
}

GLManager::~GLManager() { SDL_GL_DestroyContext(mContext); }

void GLManager::printInfo() {
	SDL_Log("Vendor     : %s\n", glGetString(GL_VENDOR));
	SDL_Log("Renderer   : %s\n", glGetString(GL_RENDERER));
	SDL_Log("Version    : %s\n", glGetString(GL_VERSION));
	SDL_Log("GLSL       : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
#ifdef DEBUG
	SDL_Log("Extensions : %s\n", glGetString(GL_EXTENSIONS));
#endif

	int maj = 0;
	int min = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &maj);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &min);
	SDL_Log("Context    : %d.%d\n", maj, min);

	glGetIntegerv(GL_MAJOR_VERSION, &maj);
	glGetIntegerv(GL_MINOR_VERSION, &min);
	SDL_Log("Context    : %d.%d\n", maj, min);

	int nrAttributes = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	SDL_Log("Maximum number of vertex attributes supported: %d\n", nrAttributes);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &nrAttributes);
	SDL_Log("Maximum number of texture units supported: %d\n", nrAttributes);
}
