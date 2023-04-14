#pragma once

#include <SFML/Graphics.hpp>
#include <State.hpp>
#include <Container.hpp>

class PauseState :public State {
public:
	PauseState (StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt, sf::Time& at);
	virtual bool handleEvent(const sf::Event& event);
private:
	sf::Sprite mBackgroundSprite;
	sf::Text mPausedText;
	GUI::Container mGUIContainer;
};