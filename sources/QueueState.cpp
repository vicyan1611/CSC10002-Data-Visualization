#include <QueueState.hpp>
#include <iostream>

QueueState::QueueState(StateStack& stack, Context context)
	:State(stack, context)
	, mQueueWorld(*context.window)
	, mGUIContainer()
{

}

void QueueState::draw()
{
	mQueueWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool QueueState::update(sf::Time dt)
{
	mQueueWorld.update(dt);
	return true;
}

bool QueueState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return false;
}