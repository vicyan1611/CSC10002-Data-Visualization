#include <Application.hpp>
#include <TitleState.hpp>
#include <MenuState.hpp>
#include <GameState.hpp>
#include <PauseState.hpp>
#include <SettingsState.hpp>
#include <StaticArrayState.hpp>
#include <DynamicArrayState.hpp>
#include <SllState.hpp>
#include <DllState.hpp>
#include <StackState.hpp>
#include <QueueState.hpp>
#include <CllState.hpp>
Application::Application()
	:mWindow(sf::VideoMode(1900, 1000), "Data Visualization - Test ver", sf::Style::Close)
	,mPlayer()
	,mTextures()
	,mFonts()
	,mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
{
	mWindow.setKeyRepeatEnabled(false);
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	mTextures.load(Textures::TitleScreen, "assets/TitleScreen.png");
	mTextures.load(Textures::ButtonNormal, "assets/ButtonNormal.png");
	mTextures.load(Textures::ButtonSelected, "assets/ButtonSelected.png");
	mTextures.load(Textures::ButtonPressed, "assets/ButtonPressed.png");
	registerStates();
	mStateStack.pushState(States::Title);
}

void Application::registerStates() {
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<StaticArrayState>(States::StaticArray);
	mStateStack.registerState<DynamicArrayState>(States::DynamicArray);
	mStateStack.registerState<SllState>(States::Sll);
	mStateStack.registerState<DllState>(States::Dll);
	mStateStack.registerState<StackState>(States::Stackk);
	mStateStack.registerState<QueueState>(States::Queuee);
	mStateStack.registerState<CllState>(States::Cll);
}

void Application::run() {
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Clock clock;
	sf::Time elapsedTime = sf::Time::Zero;
	sf::Time allTime = sf::Time::Zero;
	while (mWindow.isOpen()) {
		elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		allTime += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame, allTime);
			if (mStateStack.isEmpty())
				mWindow.close();
		}
		render();
	}
}

void Application::processInput() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt, sf::Time at) {
	mStateStack.update(dt, at);
}

void Application::render() {
	mWindow.clear();
	mStateStack.draw();
	mWindow.setView(mWindow.getDefaultView());
	mWindow.display();
}