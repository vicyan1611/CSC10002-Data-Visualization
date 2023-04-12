#pragma once

#include <State.hpp>
#include <Container.hpp>
#include <InputBox.hpp>
#include <Label.hpp>
#include <Button.hpp>
#include <vector>
#include <QueueWorld.hpp>

class QueueState : public State {
public:
	QueueState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	void handleInitBox();
	void handleAddBox();
	void handleUpdateBox();
	void handleSearchBox();
private:
	QueueWorld mQueueWorld;
	GUI::Container mGUIContainer;
	GUI::InputBox::Ptr mInitBox, mAddBox, mUpdateBox, mSearchBox;
	GUI::Button::Ptr mRandomButton, mNextButton, mPreviousButton, mDeleteButton;

	std::vector <int> mData;
	int nData = 0, id = -1;
};