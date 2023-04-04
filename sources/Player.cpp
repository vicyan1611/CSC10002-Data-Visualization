#include <Player.hpp>

struct AircraftMover {
	AircraftMover(float vx, float vy) : velocity(vx, vy) {}

	void operator() (Aircraft& aircraft, sf::Time) const {
		aircraft.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

Player::Player() {
	mKeyBinding[sf::Keyboard::A] = MoveLeft;
	mKeyBinding[sf::Keyboard::D] = MoveRight;
	mKeyBinding[sf::Keyboard::W] = MoveUp;
	mKeyBinding[sf::Keyboard::S] = MoveDown;
	const float playerSpeed = 400.f;
	mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
	mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f));
	mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
	mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed));
	
	for (auto &pair: mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

void Player::handleRealtimeInput(CommandQueue& commands) {
	for (auto &pair:mKeyBinding)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second)) {
			commands.push(mActionBinding[pair.second]);
		}
	}
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands) {
	if (event.type == sf::Event::KeyPressed) {
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second)) {
			commands.push(mActionBinding[found->second]);
		}
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key) {
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); ) {
		if (itr->second == action) {
			mKeyBinding.erase(itr++);
		}
		else {
			++itr;
		}
	}
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const {
	for (auto &pair : mKeyBinding) {
		if (pair.second == action) {
			return pair.first;
		}
	}
	return sf::Keyboard::Unknown;
}

bool Player::isRealtimeAction(Action action) {
	if (action == MoveLeft || action == MoveRight || action == MoveUp || action == MoveDown) {
		return true;
	}
	return false;
}