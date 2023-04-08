#pragma once

#include <State.hpp>
#include <StaticArrayWorld.hpp>
#include <Container.hpp>
#include <InputBox.hpp>
#include <vector>
#include <Label.hpp>
#include <Button.hpp>

class StaticArrayState : public State {
public:
	StaticArrayState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	void handleInitBox();
	void handleUpdateBox();	
	void handleSearchBox();
private:
	StaticArrayWorld mSAWorld;
	GUI::Container mGUIContainer;
	GUI::InputBox::Ptr mInitBox, mUpdateBox, mSearchBox;
	GUI::Button::Ptr mRandomButton, mNextButton, mPreviousButton;
	int nData = 0, id = -1, value = 0;
	std::vector <int> mData;
};