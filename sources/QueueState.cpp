#include <QueueState.hpp>
#include <iostream>

QueueState::QueueState(StateStack& stack, Context context)
	:State(stack, context)
	, mQueueWorld(*context.window)
	, mGUIContainer()
{
	mInitBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mInitBox->setPosition(100, 650);
	mInitBox->setText("");
	mGUIContainer.pack(mInitBox);
	GUI::Label::Ptr mInitLabel = std::make_shared<GUI::Label>("Initialize Box", *context.fonts);
	mInitLabel->setPosition(100, 620);
	mGUIContainer.pack(mInitLabel);
}

void QueueState::handleInitBox() {
	std::string temp = mInitBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		if (nData == 0) nData = x + 1; else if (nData > 1) {
			nData--;
			mData.push_back(x);
		}
		if (nData == 1) {
			nData = 0;
			mQueueWorld.setArray(mData);
			mData.clear();
		}
	}
}

void QueueState::draw()
{
	mQueueWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool QueueState::update(sf::Time dt)
{
	mQueueWorld.update(dt);
	return true;
}

bool QueueState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	handleInitBox();
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return false;
}