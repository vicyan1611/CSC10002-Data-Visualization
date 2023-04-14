#pragma once
#include <SFML/Graphics.hpp>
#include <StateStack.hpp>
#include <World.hpp>
#include <Player.hpp>
#include <CommandQueue.hpp>

class Application {
public:
	Application();
	void run();
private:
	void processInput();
	void update(sf::Time dt, sf::Time at);
	void render();
	void registerStates();
private:
	sf::RenderWindow mWindow;
	Player mPlayer;
	StateStack mStateStack;
	TextureHolder mTextures;
	FontHolder mFonts;

	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
};