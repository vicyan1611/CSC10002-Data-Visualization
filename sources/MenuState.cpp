#include <MenuState.hpp>
#include <Button.hpp>
#include <InputBox.hpp>
#include <ResourceHolder.hpp>
#include <SFML/Graphics.hpp>

MenuState::MenuState(StateStack& stack, Context context)
	:State(stack, context)
	, mGUIContainer()
	, mBackgroundID(*context.id)
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto changeBackGroundButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	changeBackGroundButton->setPosition(1600, 200);
	changeBackGroundButton->setText("Change Background");
	changeBackGroundButton->setCallback([this]() {
		changeBackground();
		});
	mGUIContainer.pack(changeBackGroundButton);

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

	auto daButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	daButton->setPosition(100, 450);
	daButton->setText("Dynamic Array");
	daButton->setCallback([this]() {
		requestStackPush(States::DynamicArray);
		});
	mGUIContainer.pack(daButton);

	auto sllButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	sllButton->setPosition(100, 500);
	sllButton->setText("Singly Linked List");
	sllButton->setCallback([this]() {
		requestStackPush(States::Sll);
		});
	mGUIContainer.pack(sllButton);

	auto dllButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	dllButton->setPosition(100, 550);
	dllButton->setText("Doubly Linked List");
	dllButton->setCallback([this]() {
		requestStackPush(States::Dll);
		});
	mGUIContainer.pack(dllButton);

	auto cllButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	cllButton->setPosition(100, 600);
	cllButton->setText("Circular Linked List");
	cllButton->setCallback([this]() {
		requestStackPush(States::Cll);
		});
	mGUIContainer.pack(cllButton);

	auto stackButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	stackButton->setPosition(100, 650);
	stackButton->setText("Stack");
	stackButton->setCallback([this]() {
		requestStackPush(States::Stackk);
		});
	mGUIContainer.pack(stackButton);

	auto queueButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	queueButton->setPosition(100, 700);
	queueButton->setText("Queue");
	queueButton->setCallback([this]() {
		requestStackPush(States::Queuee);
		});
	mGUIContainer.pack(queueButton);

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

bool MenuState::update(sf::Time dt, sf::Time& at) {
	return true;
}

bool MenuState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}

void MenuState::changeBackground() {
	mBackgroundID++;
	if (mBackgroundID > 5) mBackgroundID = 0;
	setBackground();
}

void MenuState::setBackground() {
	switch (mBackgroundID) {
	case 0:
		mBackgroundSprite.setTexture(getContext().textures->get(Textures::TitleScreen));
		break;
	case 1:
		mBackgroundSprite.setTexture(getContext().textures->get(Textures::TitleScreen1));
		break;
	case 2:
		mBackgroundSprite.setTexture(getContext().textures->get(Textures::TitleScreen2));
		break;
	case 3:
		mBackgroundSprite.setTexture(getContext().textures->get(Textures::TitleScreen3));
		break;
	case 4:
		mBackgroundSprite.setTexture(getContext().textures->get(Textures::TitleScreen4));
		break;
	case 5:
		mBackgroundSprite.setTexture(getContext().textures->get(Textures::TitleScreen5));
		break;
	}
}