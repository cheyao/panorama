#pragma once

#include "third_party/glad/glad.h"

#include <string>

class Texture {
  public:
	explicit Texture(const std::string_view& path);
	Texture(Texture&&) = delete;
	Texture(const Texture&) = delete;
	Texture& operator=(Texture&&) = delete;
	Texture& operator=(const Texture&) = delete;
	virtual ~Texture();

	virtual void activate(const unsigned int& num) const;
	virtual void load();

  protected:
	GLuint mID;
	std::string name;
};
