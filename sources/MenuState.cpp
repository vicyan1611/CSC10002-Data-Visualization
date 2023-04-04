#include <MenuState.hpp>
#include <Button.hpp>
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

bool MenuState::handleEvent(const sf::Event & event) {
	mGUIContainer.handleEvent(event);
	return false;
}