#pragma once

#include <State.hpp>

class StaticArrayState : public State {
public:
	StaticArrayState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	sf::Sprite mBackgroundSprite;
};