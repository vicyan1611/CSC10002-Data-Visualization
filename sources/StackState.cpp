#include <StackState.hpp>
#include <iostream>

StackState::StackState(StateStack& stack, Context context)
	:State(stack, context)
	, mStackWorld(*context.window)
	, mGUIContainer()
{
}

void StackState::draw()
{
	mStackWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool StackState::update(sf::Time dt)
{
	mStackWorld.update(dt);
	return true;
}

bool StackState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return false;
}