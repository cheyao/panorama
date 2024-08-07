#include "opengl/cubemap.hpp"

#include "third_party/stb_image.h"
#include "utils.hpp"

#include <SDL3/SDL.h>
#include <filesystem>
#include <string_view>
#include <vector>

Cubemap::Cubemap(const std::string_view& path) : Texture(path) {}

void Cubemap::activate(const unsigned int& num) const {
	glActiveTexture(GL_TEXTURE0 + num);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
}

void Cubemap::load() {
	SDL_Log("Loading cubemap %s", name.data());

	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mID);

	const std::vector faces0 = {"right.png",  "left.png",  "top.png",
								"bottom.png", "front.png", "back.png"};
	const std::vector faces1 = {"right.jpg",  "left.jpg",  "top.jpg",
								"bottom.jpg", "front.jpg", "back.jpg"};
	const std::vector faces2 = {"panorama_0.png", "panorama_1.png", "panorama_2.png",
								"panorama_3.png", "panorama_4.png", "panorama_5.png"};
	const std::vector faces3 = {"panorama_0.jpg", "panorama_1.jpg", "panorama_2.jpg",
								"panorama_3.jpg", "panorama_4.jpg", "panorama_5.jpg"};

	if (std::filesystem::exists(name + "right.png")) {
		for (unsigned int i = 0; i < faces0.size(); i++) {
			loadface(faces0[i], i);
		}
	} else if (std::filesystem::exists(name + "right.jpg")) {
		for (unsigned int i = 0; i < faces1.size(); i++) {
			loadface(faces1[i], i);
		}
	} else if (std::filesystem::exists(name + "panorama_0.png")) {
		for (unsigned int i = 0; i < faces2.size(); i++) {
			loadface(faces2[i], i);
		}
	} else if (std::filesystem::exists(name + "panorama_0.jpg")) {
		for (unsigned int i = 0; i < faces3.size(); i++) {
			loadface(faces3[i], i);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	SDL_Log("Loaded cubemap %s", name.data());
}

void Cubemap::loadface(const std::string& face, const unsigned int& i) {
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = stbi_load((name + face).data(), &width, &height, &channels, 0);

	[[unlikely]] if (data == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s\n",
					 (name + face).data());
		ERROR_BOX("Failed to load textures, the assets is corrupted or you don't "
				  "have enough memory");

		throw std::runtime_error("cubemap.cpp: Failed to load texture");
	}

	GLenum format = GL_RGB;
	switch (channels) {
		// TODO: Gray scale
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		[[unlikely]] default:
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s:%d Unimplemented image format: %s\n",
						 __FILE__, __LINE__, (name + face).data());
			ERROR_BOX("Failed to recognise file color format, the assets is probably "
					  "corrupted");

			throw std::runtime_error("cubemap.cpp: Invalid enum");
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format,
				 GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}
