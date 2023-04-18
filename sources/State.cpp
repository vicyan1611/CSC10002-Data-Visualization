#include <State.hpp>
#include <StateStack.hpp>

State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)
{
}

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player, int& id)
	:window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, player(&player)
	, id(&id)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID) {
	mStack->pushState(stateID);
}

void State::requestStackPop() {
	mStack->popState();
}

void State::requestStateClear() {
	mStack->clearStates();
}

State::Context State::getContext() const {
	return mContext;
}