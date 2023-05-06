#include <QueueState.hpp>
#include <iostream>

QueueState::QueueState(StateStack& stack, Context context)
	:State(stack, context)
	, mQueueWorld(*context.window, *context.id)
	, mGUIContainer()
{
	mRandomButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mRandomButton->setPosition(100, 750);
	mRandomButton->setText("Random Data");
	mRandomButton->setCallback([this]() {
		mQueueWorld.setRandomArray();
		});
	mGUIContainer.pack(mRandomButton);

	mFileLoadButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mFileLoadButton->setPosition(300, 750);
	mFileLoadButton->setText("Load From File");
	mFileLoadButton->setCallback([this]() {
		mQueueWorld.loadFromFile();
		});
	mGUIContainer.pack(mFileLoadButton);

	mClearButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mClearButton->setPosition(500, 750);
	mClearButton->setText("Clear");
	mClearButton->setCallback([this]() {
		mQueueWorld.clear();
		});
	mGUIContainer.pack(mClearButton);

	mDeleteButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mDeleteButton->setPosition(700, 750);
	mDeleteButton->setText("DeQueue");
	mDeleteButton->setCallback([this]() {
		mQueueWorld.dequeue();
		});
	mGUIContainer.pack(mDeleteButton);

	mInitBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mInitBox->setPosition(100, 850);
	mInitBox->setText("");
	mGUIContainer.pack(mInitBox);
	GUI::Label::Ptr mInitLabel = std::make_shared<GUI::Label>("Initialize Box", *context.fonts);
	mInitLabel->setPosition(100, 820);
	mGUIContainer.pack(mInitLabel);

	mAddBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mAddBox->setPosition(300, 850);
	mAddBox->setText("");
	mGUIContainer.pack(mAddBox);
	GUI::Label::Ptr mAddLabel = std::make_shared<GUI::Label>("EnQueue", *context.fonts);
	mAddLabel->setPosition(300, 820);
	mGUIContainer.pack(mAddLabel);

	mUpdateBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mUpdateBox->setPosition(500, 850);
	mUpdateBox->setText("");
	mGUIContainer.pack(mUpdateBox);
	GUI::Label::Ptr mUpdateLabel = std::make_shared<GUI::Label>("Update", *context.fonts);
	mUpdateLabel->setPosition(500, 820);
	mGUIContainer.pack(mUpdateLabel);

	mSearchBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mSearchBox->setPosition(700, 850);
	mSearchBox->setText("");
	mGUIContainer.pack(mSearchBox);
	GUI::Label::Ptr mSearchLabel = std::make_shared<GUI::Label>("Search", *context.fonts);
	mSearchLabel->setPosition(700, 820);
	mGUIContainer.pack(mSearchLabel);

	mRunAtOnceButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mRunAtOnceButton->setPosition(1600, 750);
	mRunAtOnceButton->setText("Run at once");
	mRunAtOnceButton->setCallback([this]() {
		mQueueWorld.runAtOnce();
		});
	mGUIContainer.pack(mRunAtOnceButton);

	mPreviousButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mPreviousButton->setPosition(1600, 800);
	mPreviousButton->setText("Previous");
	mPreviousButton->setCallback([this]() {
		mQueueWorld.previous();
		});
	mGUIContainer.pack(mPreviousButton);

	mNextButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mNextButton->setPosition(1600, 850);
	mNextButton->setText("Next");
	mNextButton->setCallback([this]() {
		mQueueWorld.next();
		});
	mGUIContainer.pack(mNextButton);
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

bool QueueState::update(sf::Time dt, sf::Time& at)
{
	mQueueWorld.update(dt, at);
	return true;
}

void QueueState::handleAddBox() {
	std::string temp = mAddBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		mQueueWorld.enqueue(x);
	}
}

void QueueState::handleUpdateBox() {
	std::string temp = mUpdateBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		if (id == -1) id = x; else {
			mQueueWorld.updateQueue(id, x);
			id = -1;
		}
	}
}

void QueueState::handleSearchBox() {
	std::string temp = mSearchBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		mQueueWorld.searchQueue(x);
	}
}

bool QueueState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	handleInitBox();
	handleAddBox();
	handleUpdateBox();
	handleSearchBox();
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return false;
}