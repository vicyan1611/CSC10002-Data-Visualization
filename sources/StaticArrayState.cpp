#include <StaticArrayState.hpp>

StaticArrayState::StaticArrayState(StateStack& stack, Context context)
	:State(stack, context)
	, mSAWorld(*context.window)
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
		mSAWorld.setRandomArray();
	});
	mGUIContainer.pack(mRandomButton);

	mUpdateBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mUpdateBox->setPosition(700, 650);
	mUpdateBox->setText("");
	mGUIContainer.pack(mUpdateBox);
	GUI::Label::Ptr mUpdateLabel = std::make_shared<GUI::Label>("Update Box", *context.fonts);
	mUpdateLabel->setPosition(700, 620);
	mGUIContainer.pack(mUpdateLabel);

	mSearchBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mSearchBox->setPosition(1000, 650);
	mSearchBox->setText("");
	mGUIContainer.pack(mSearchBox);
	GUI::Label::Ptr mSearchLabel = std::make_shared<GUI::Label>("Search Box", *context.fonts);
	mSearchLabel->setPosition(1000, 620);
	mGUIContainer.pack(mSearchLabel);

	mRunAtOnceButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mRunAtOnceButton->setPosition(1000, 700);
	mRunAtOnceButton->setText("Run at once");
	mRunAtOnceButton->setCallback([this]() {
		mSAWorld.runAtOnce();
		});
	mGUIContainer.pack(mRunAtOnceButton);

	mPreviousButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mPreviousButton->setPosition(1000, 750);
	mPreviousButton->setText("Previous");
	mPreviousButton->setCallback([this]() {
		mSAWorld.previous();
		});
	mGUIContainer.pack(mPreviousButton);

	mNextButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mNextButton->setPosition(1000, 800);
	mNextButton->setText("Next");
	mNextButton->setCallback([this]() {
		mSAWorld.next();
	});
	mGUIContainer.pack(mNextButton);
}

void StaticArrayState::draw() {
	mSAWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

void StaticArrayState::handleInitBox() {
	std::string temp = mInitBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		std::cout << x << std::endl;
		if (nData == 0) nData = x + 1; else if (nData > 1) {
			nData--;
			mData.push_back(x);
		}
		if (nData == 1) {
			mSAWorld.setArray(mData);
			mData.clear();
			nData = 0;
		}
	}
}

void StaticArrayState::handleUpdateBox() {
	std::string temp = mUpdateBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		if (id == -1) id = x; else {
			value = x;
			mSAWorld.updateArray(id, value);
			id = -1;
		}
	}
}

void StaticArrayState::handleSearchBox() {
	std::string temp = mSearchBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		std::cout << x << std::endl;
		mSAWorld.searchArray(x);
	}
}

bool StaticArrayState::update(sf::Time dt, sf::Time& at) {
	mSAWorld.update(dt, at);
	//CommandQueue& commands = mWorld.getCommandQueue();
	return true;
}

bool StaticArrayState::handleEvent(const sf::Event& event) {
	//CommandQueue& commands = mWorld.getCommandQueue();
	mGUIContainer.handleEvent(event);
	handleInitBox();
	handleUpdateBox();
	handleSearchBox();
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		requestStackPush(States::Pause);
	}
	return false;
}