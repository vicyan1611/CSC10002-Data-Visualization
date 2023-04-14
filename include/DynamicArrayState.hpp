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
	virtual bool update(sf::Time dt, sf::Time& at);
	virtual bool handleEvent(const sf::Event& event);
private:
	void handleInitBox();
	void handleAddBox();
	void handleDeleteBox();
	void handleUpdateBox();
	void handleSearchBox();
private:
    DynamicArrayWorld mDAWorld;
	GUI::Container mGUIContainer;
	GUI::InputBox::Ptr mInitBox, mUpdateBox, mSearchBox, mAddBox, mDeleteBox;
	GUI::Button::Ptr mRandomButton, mNextButton, mPreviousButton, mRunAtOnceButton, mFileLoadButton;
	std::vector <int> mData;
	int nData = 0, id = -1, value = 0;
};