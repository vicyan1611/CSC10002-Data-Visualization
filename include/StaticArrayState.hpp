#pragma once

#include <State.hpp>
#include <StaticArrayWorld.hpp>
#include <Container.hpp>
#include <InputBox.hpp>
#include <vector>
class StaticArrayState : public State {
public:
	StaticArrayState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	StaticArrayWorld mSAWorld;
	GUI::Container mGUIContainer;
	sf::Sprite mBackgroundSprite;
	GUI::InputBox::Ptr mInputBox;
	int nData;
	std::vector <int> Data;
};