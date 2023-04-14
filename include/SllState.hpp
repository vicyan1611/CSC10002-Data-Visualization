#pragma once

#include <State.hpp>
#include <Container.hpp>
#include <InputBox.hpp>
#include <Label.hpp>
#include <Button.hpp>
#include <vector>
#include <SllWorld.hpp>

class SllState : public State {
public:
	SllState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt, sf::Time at);
	virtual bool handleEvent(const sf::Event& event);
private:
	void handleInitBox();
	void handleAddBox();
	void handleDeleteBox();
	void handleUpdateBox();
	void handleSearchBox();
private:
	SllWorld mSllWorld;
	GUI::Container mGUIContainer;
	GUI::InputBox::Ptr mInitBox, mUpdateBox, mSearchBox, mAddBox, mDeleteBox;
	GUI::Button::Ptr mRandomButton, mNextButton, mPreviousButton;
	std::vector <int> mData;
	int nData = 0, id = -1;
};