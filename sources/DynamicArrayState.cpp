#include <DynamicArrayState.hpp>

DynamicArrayState::DynamicArrayState(StateStack& stack, Context context)
	:State(stack, context)
	, mDAWorld(*context.window)
	, mGUIContainer()
{

}

void DynamicArrayState::draw() {
	mDAWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool DynamicArrayState::update(sf::Time dt) {
	mDAWorld.update(dt);
	return true;
}

bool DynamicArrayState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		requestStackPush(States::Pause);
	}
	return false;
}