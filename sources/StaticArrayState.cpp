#include <StaticArrayState.hpp>

StaticArrayState::StaticArrayState(StateStack& stack, Context context)
	:State(stack, context)
	, mSAWorld(*context.window)
	, mGUIContainer()
{
	mBackgroundSprite = sf::Sprite(context.textures->get(Textures::TitleScreen));

    mInputBox = std::make_shared<GUI::InputBox>(*context.fonts);
	mInputBox->setPosition(100, 450);
	mInputBox->setText("");
	mGUIContainer.pack(mInputBox);
	
}

void StaticArrayState::draw() {
	//mSAWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool StaticArrayState::update(sf::Time dt) {
	//mWorld.update(dt);
	//CommandQueue& commands = mWorld.getCommandQueue();
	std::string temp = mInputBox->getFinalText();
	if (temp != "") {
		std::cout << temp << std::endl;
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