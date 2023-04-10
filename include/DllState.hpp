#pragma once

#include <State.hpp>
#include <Container.hpp>
#include <InputBox.hpp>
#include <Label.hpp>
#include <Button.hpp>
#include <vector>
#include <DllWorld.hpp>

class DllState: public State {
public:
	DllState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	void handleInitBox();
	void handleAddBox();
	void handleDeleteBox();
	void handleUpdateBox();
	void handleSearchBox();
private:
	DllWorld mDllWorld;
	GUI::Container mGUIContainer;
	GUI::InputBox::Ptr mInitBox, mUpdateBox, mSearchBox, mAddBox, mDeleteBox;
	GUI::Button::Ptr mRandomButton, mNextButton, mPreviousButton;
	std::vector <int> mData;
	int nData = 0, id = -1;
};