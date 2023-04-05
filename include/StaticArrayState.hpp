#pragma once

#include <State.hpp>
#include <StaticArrayWorld.hpp>

class StaticArrayState : public State {
public:
	StaticArrayState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	StaticArrayWorld mWorld;
};