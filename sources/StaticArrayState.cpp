#include <StaticArrayState.hpp>

StaticArrayState::StaticArrayState(StateStack& stack, Context context)
	:State(stack, context)
	, mWorld(*context.window)
{
}

void StaticArrayState::draw() {
	mWorld.draw();
}

bool StaticArrayState::update(sf::Time dt) {
	mWorld.update(dt);
	CommandQueue& commands = mWorld.getCommandQueue();
	
	return true;
}

bool StaticArrayState::handleEvent(const sf::Event& event) {
	CommandQueue& commands = mWorld.getCommandQueue();
	
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		requestStackPush(States::Pause);
	}
	return true;
}