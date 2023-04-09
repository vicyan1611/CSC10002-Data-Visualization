#include <SllState.hpp>
#include <iostream>

SllState::SllState(StateStack& stack, Context context)
	:State(stack, context)
	, mSllWorld(*context.window)
	, mGUIContainer()
{

}

void SllState::draw() {
	mSllWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool SllState::update(sf::Time dt) {
	mSllWorld.update(dt);
	return true;
}

bool SllState::handleEvent(const sf::Event& event) {
	 mGUIContainer.handleEvent(event);
	 if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		 requestStackPush(States::Pause);
	 }
	return false;
}