#include <StaticArrayState.hpp>

StaticArrayState::StaticArrayState(StateStack& stack, Context context)
	:State(stack, context)
	, mSAWorld(*context.window)
	, mGUIContainer()
{
	mBackgroundSprite = sf::Sprite(context.textures->get(Textures::TitleScreen));
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

}

void StaticArrayState::draw() {
	mSAWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	//window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool StaticArrayState::update(sf::Time dt) {
	mSAWorld.update(dt);
	//CommandQueue& commands = mWorld.getCommandQueue();
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
	return true;
}

bool StaticArrayState::handleEvent(const sf::Event& event) {
	//CommandQueue& commands = mWorld.getCommandQueue();
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		requestStackPush(States::Pause);
	}
	return false;
}