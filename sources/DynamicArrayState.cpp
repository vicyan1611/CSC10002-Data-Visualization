#include <DynamicArrayState.hpp>

DynamicArrayState::DynamicArrayState(StateStack& stack, Context context)
	:State(stack, context)
	, mDAWorld(*context.window)
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
		mDAWorld.setRandomArray();
		});
	mGUIContainer.pack(mRandomButton);
}

void DynamicArrayState::draw() {
	mDAWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool DynamicArrayState::update(sf::Time dt) {
	mDAWorld.update(dt);
	return true;
}

void DynamicArrayState::handleInitBox() {
	std::string temp = mInitBox->getFinalText();
	if (temp != "") {
		int x = std::stoi(temp);
		if (nData == 0) nData = x + 1; else if (nData > 1) {
			nData--;
			mData.push_back(x);
		}
		if (nData == 1) {
			nData = 0;
			mDAWorld.setArray(mData);
			mData.clear();
		}
	}
}

bool DynamicArrayState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	handleInitBox();
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		requestStackPush(States::Pause);
	}
	return false;
}