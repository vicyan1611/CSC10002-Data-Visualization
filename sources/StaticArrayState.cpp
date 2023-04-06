#include <StaticArrayState.hpp>
#include <InputBox.hpp>
StaticArrayState::StaticArrayState(StateStack& stack, Context context)
	:State(stack, context)
	, mSAWorld(*context.window)
	, mGUIContainer()
{
	mBackgroundSprite = sf::Sprite(context.textures->get(Textures::TitleScreen));

	auto userInput = std::make_shared<GUI::InputBox>(*context.fonts);
	userInput->setPosition(100, 450);
	userInput->setText("");
	mGUIContainer.pack(userInput);
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