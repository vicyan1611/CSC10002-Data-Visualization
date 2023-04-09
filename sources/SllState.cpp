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
