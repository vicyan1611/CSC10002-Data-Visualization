#include <SllState.hpp>
#include <iostream>

SllState::SllState(StateStack& stack, Context context)
	:State(stack, context)
	, mSllWorld(*context.window)
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

void SllState::draw() {
	mSllWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool SllState::update(sf::Time dt) {
	mSllWorld.update(dt);
	return true;
}

void SllState::handleInitBox() {
	std::string temp = mInitBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		if (nData == 0) nData = x + 1; else if (nData > 1) {
			nData--;
			mData.push_back(x);
		}
		if (nData == 1) {
			nData = 0;
			mSllWorld.setArray(mData);
			mData.clear();
		}
	}
}

bool SllState::handleEvent(const sf::Event& event) {
	 mGUIContainer.handleEvent(event);
	// handleInitBox();
	 if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		 requestStackPush(States::Pause);
	 }
	return false;
}