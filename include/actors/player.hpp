#pragma once

#include "actors/actor.hpp"

class Player : public Actor {
public:
	explicit Player(class Game* owner);
	Player(Player &&) = delete;
	Player(const Player &) = delete;
	Player &operator=(Player &&) = delete;
	Player &operator=(const Player &) = delete;
	/*~Player();*/

	void actorInput(const uint8_t* keystate) override;

private:
	class MovementComponent* mMoveComp;
};

