#include <MenuState.hpp>
#include <Button.hpp>
#include <InputBox.hpp>
#include <ResourceHolder.hpp>
#include <SFML/Graphics.hpp>

MenuState::MenuState(StateStack& stack, Context context)
	:State(stack, context)
	, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	playButton->setPosition(100, 250);
	playButton->setText("Play");
	playButton->setCallback([this]() {
		requestStackPop();
		requestStackPush(States::Game);
		});
	mGUIContainer.pack(playButton);

	auto settingsButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	settingsButton->setPosition(100, 300);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]() {
		requestStackPush(States::Settings);
		});
	mGUIContainer.pack(settingsButton);

	auto exitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	exitButton->setPosition(100, 350);
	exitButton->setText("Exit");
	exitButton->setCallback([this]() {
		requestStackPop();
		});
	mGUIContainer.pack(exitButton);

	auto saButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	saButton->setPosition(100, 400);
	saButton->setText("Static Array");
	saButton->setCallback([this]() {
		requestStackPush(States::StaticArray);
		});
	mGUIContainer.pack(saButton);

	/*auto userInput = std::make_shared<GUI::InputBox>(*context.fonts);
	userInput->setPosition(100, 450);
	userInput->setText("Enter your name");
	mGUIContainer.pack(userInput);*/
}

void MenuState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time dt) {
	return true;
}

bool MenuState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}