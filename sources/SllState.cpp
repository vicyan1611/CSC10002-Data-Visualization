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

	mRandomButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mRandomButton->setPosition(400, 650);
	mRandomButton->setText("Random Data");
	mRandomButton->setCallback([this]() {
		mSllWorld.setRandomArray();
		});
	mGUIContainer.pack(mRandomButton);

	mAddBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mAddBox->setPosition(700, 650);
	mAddBox->setText("");
	mGUIContainer.pack(mAddBox);
	GUI::Label::Ptr mAddLabel = std::make_shared<GUI::Label>("Add Box", *context.fonts);
	mAddLabel->setPosition(700, 620);
	mGUIContainer.pack(mAddLabel);

	mDeleteBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mDeleteBox->setPosition(1000, 650);
	mDeleteBox->setText("");
	mGUIContainer.pack(mDeleteBox);
	GUI::Label::Ptr mDeleteLabel = std::make_shared<GUI::Label>("Delete Box", *context.fonts);
	mDeleteLabel->setPosition(1000, 620);
	mGUIContainer.pack(mDeleteLabel);

	mPreviousButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mPreviousButton->setPosition(1000, 720);
	mPreviousButton->setText("Previous");
	mPreviousButton->setCallback([this]() {
		mSllWorld.previous();
		});
	mGUIContainer.pack(mPreviousButton);

	mNextButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mNextButton->setPosition(1000, 800);
	mNextButton->setText("Next");
	mNextButton->setCallback([this]() {
		mSllWorld.next();
		});
	mGUIContainer.pack(mNextButton);
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

void SllState::handleAddBox() {
	std::string temp = mAddBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		if (id == -1) id = x; else {
			mSllWorld.addToArray(id, x);
			id = -1;
		}
	}
}

void SllState::handleDeleteBox() {
	std::string temp = mDeleteBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		mSllWorld.deleteFromArray(x);
	}
}

bool SllState::handleEvent(const sf::Event& event) {
	 mGUIContainer.handleEvent(event);
	 handleInitBox();
	 handleAddBox();
	 handleDeleteBox();
	 if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		 requestStackPush(States::Pause);
	 }
	return false;
}