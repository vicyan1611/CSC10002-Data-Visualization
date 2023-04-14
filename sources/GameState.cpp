#include <GameState.hpp>

GameState::GameState(StateStack& stack, Context context)
	:State(stack, context)
	, mWorld(*context.window)
	, mPlayer(*context.player)
{
}

void GameState::draw() {
	mWorld.draw();
}

bool GameState::update(sf::Time dt, sf::Time& at) {
	mWorld.update(dt, at);

	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event) {
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		requestStackPush(States::Pause);
	}
	return true;
}