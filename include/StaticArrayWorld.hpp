#pragma once

#include <SFML/Graphics.hpp>
#include <ResourceHolder.hpp>
#include <SceneNode.hpp>
#include <Aircraft.hpp>
#include <array>
#include <SpriteNode.hpp>
#include <CommandQueue.hpp>
#include <vector>

class StaticArrayWorld : private sf::NonCopyable {
public:
	explicit StaticArrayWorld(sf::RenderWindow& window, int& id);
	void update(sf::Time dt, sf::Time& at);
	void draw();
	CommandQueue& getCommandQueue();
	void setArray(std::vector<int> data);
	void setRandomArray();
	void updateArray(int id, int value);
	void searchArray(int value);
	void next();
	void previous();
	void runAtOnce();
	void loadFromFile();
private:
	void loadTextures(int id);
	void buildScene();
	void searchArrayStep();
private:
	enum Layer {
		Background,
		Air,
		LayerCount
	};
private:
	sf::RenderWindow& mWindow;
	sf::View mWorldView;
	TextureHolder mTextures;
	FontHolder mFonts;
	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;

	sf::FloatRect mWorldBounds;
	std::vector<Aircraft*> mPlayerAircraftar;
	CommandQueue mCommandQueue;
	int totalSearchStep = 0, step = -1, searchValue = 0, operationType = 0;
	bool isRunAtOnce = false;
};