#include <CllState.hpp>
#include <iostream>

CllState::CllState(StateStack& stack, Context context)
	:State(stack, context)
	, mCllWorld(*context.window)
	, mGUIContainer()
{

}

void CllState::draw()
{
	mCllWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool CllState::update(sf::Time dt)
{
	mCllWorld.update(dt);
	return true;
}

bool CllState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return false;
}