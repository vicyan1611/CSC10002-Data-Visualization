#pragma once

#include <State.hpp>
#include <Container.hpp>
#include <InputBox.hpp>
#include <Label.hpp>
#include <Button.hpp>
#include <vector>
#include <StackWorld.hpp>

class StackState : public State {
public:
	StackState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt, sf::Time& at);
	virtual bool handleEvent(const sf::Event& event);
private:
	void handleInitBox();
	void handleAddBox();
	void handleUpdateBox();
	void handleSearchBox();
private:
	StackWorld mStackWorld;
	GUI::Container mGUIContainer;
	GUI::InputBox::Ptr mInitBox, mAddBox, mUpdateBox, mSearchBox;
	GUI::Button::Ptr mRandomButton, mNextButton, mPreviousButton, mDeleteButton, mRunAtOnceButton, mFileLoadButton;

	std::vector <int> mData;
	int nData = 0, id = -1;
};