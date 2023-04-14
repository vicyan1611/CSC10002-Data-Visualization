#pragma once

#include <State.hpp>
#include <Container.hpp>


class MenuState : public State {
public:
	MenuState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt, sf::Time& at);
	virtual bool handleEvent(const sf::Event& event);
private:
private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
};