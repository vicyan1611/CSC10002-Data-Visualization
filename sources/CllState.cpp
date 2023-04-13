#include <CllState.hpp>
#include <iostream>

CllState::CllState(StateStack& stack, Context context)
	:State(stack, context)
	, mCllWorld(*context.window)
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
		mCllWorld.setRandomArray();
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

	mUpdateBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mUpdateBox->setPosition(1300, 650);
	mUpdateBox->setText("");
	mGUIContainer.pack(mUpdateBox);
	GUI::Label::Ptr mUpdateLabel = std::make_shared<GUI::Label>("Update Box", *context.fonts);
	mUpdateLabel->setPosition(1300, 620);
	mGUIContainer.pack(mUpdateLabel);

	mSearchBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mSearchBox->setPosition(1600, 650);
	mSearchBox->setText("");
	mGUIContainer.pack(mSearchBox);
	GUI::Label::Ptr mSearchLabel = std::make_shared<GUI::Label>("Search Box", *context.fonts);
	mSearchLabel->setPosition(1600, 620);
	mGUIContainer.pack(mSearchLabel);

	mPreviousButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mPreviousButton->setPosition(1600, 720);
	mPreviousButton->setText("Previous");
	mPreviousButton->setCallback([this]() {
		mCllWorld.previous();
		});
	mGUIContainer.pack(mPreviousButton);

	mNextButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mNextButton->setPosition(1600, 800);
	mNextButton->setText("Next");
	mNextButton->setCallback([this]() {
		mCllWorld.next();
		});
	mGUIContainer.pack(mNextButton);
}

void CllState::draw()
{
	mCllWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool CllState::update(sf::Time dt)
{
	mCllWorld.update(dt);
	return true;
}

void CllState::handleInitBox() {
	std::string temp = mInitBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		if (nData == 0) nData = x + 1; else if (nData > 1) {
			nData--;
			mData.push_back(x);
		}
		if (nData == 1) {
			nData = 0;
			mCllWorld.setArray(mData);
			mData.clear();
		}
	}
}

void CllState::handleAddBox() {
	std::string temp = mAddBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		if (id == -1) id = x; else {
			mCllWorld.addToArray(id, x);
			id = -1;
		}
	}
}

void CllState::handleDeleteBox() {
	std::string temp = mDeleteBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		mCllWorld.deleteFromArray(x);
	}
}

void CllState::handleUpdateBox() {
	std::string temp = mUpdateBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		if (id == -1) id = x; else {
			mCllWorld.updateArray(id, x);
			id = -1;
		}
	}
}

void CllState::handleSearchBox() {
	std::string temp = mSearchBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		mCllWorld.searchArray(x);
	}
}

bool CllState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	handleInitBox();
	handleAddBox();
	handleDeleteBox();
	handleUpdateBox();
	handleSearchBox();
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return false;
}