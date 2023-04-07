#pragma once

#include <State.hpp>
#include <DynamicArrayWorld.hpp>
#include <Container.hpp>
#include <InputBox.hpp>
#include <vector>
#include <Label.hpp>
#include <Button.hpp>

class DynamicArrayState : public State {
public:
	DynamicArrayState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	void updateInitBox();
	void updateUpdateBox();
	void updateSearchBox();
private:
    DynamicArrayWorld mDAWorld;
	GUI::Container mGUIContainer;
	GUI::InputBox::Ptr mInitBox, mUpdateBox, mSearchBox;
	GUI::Button::Ptr mRandomButton, mNextButton, mPreviousButton;
};