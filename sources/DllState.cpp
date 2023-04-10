#include <DllState.hpp>
#include <iostream>

DllState::DllState(StateStack& stack, Context context)
	:State(stack, context)
	, mDllWorld(*context.window)
	, mGUIContainer()
{

}

void DllState::draw() {
	mDllWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool DllState::update(sf::Time dt) {
	mDllWorld.update(dt);
	return true;
}

bool DllState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return false;

}