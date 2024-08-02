#pragma once

#include "actors/actor.hpp"

#include <string>

class World : public Actor {
  public:
	explicit World(class Game* owner, std::string pano);
	World(World&&) = delete;
	World(const World&) = delete;
	World& operator=(World&&) = delete;
	World& operator=(const World&) = delete;
	~World() override = default;

  private:
};
