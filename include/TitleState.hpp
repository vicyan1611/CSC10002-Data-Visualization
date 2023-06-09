#pragma once
#include <State.hpp>

class TitleState : public State {
public:
	TitleState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt, sf::Time& at);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite;
	sf::Text mText, author, title;
	bool mShowText;
	sf::Time mTextEffectTime;
};