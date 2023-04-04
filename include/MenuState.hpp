#pragma once

#include <State.hpp>
#include <Container.hpp>


class MenuState : public State {
public:
	MenuState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	enum OptionNames {
		Play,
		Exit,
	};
	private:
		sf::Sprite mBackgroundSprite;
		GUI::Container mGUIContainer;
};