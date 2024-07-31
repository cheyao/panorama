#pragma once

#include <string>
#include <unordered_map>

#ifdef DEBUG
#include <filesystem>
#endif

class ShaderManager {
  public:
	explicit ShaderManager(const std::string& path);
	ShaderManager(ShaderManager&&) = delete;
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(ShaderManager&&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
	~ShaderManager();

	class Shader* get(const std::string& vert, const std::string& frag);

	void reload(bool full = false);

  private:
	std::unordered_map<std::string, class Shader*> mTextures;

	std::string mPath;

#ifdef DEBUG
	std::unordered_map<class Shader*, std::filesystem::file_time_type> mLastEdit;
#endif
};
