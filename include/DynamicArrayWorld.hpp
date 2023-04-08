#pragma once

#include <SFML/Graphics.hpp>
#include <ResourceHolder.hpp>
#include <SceneNode.hpp>
#include <Aircraft.hpp>
#include <array>
#include <SpriteNode.hpp>
#include <CommandQueue.hpp>
#include <vector>

class DynamicArrayWorld : private sf::NonCopyable {
public:
	explicit DynamicArrayWorld(sf::RenderWindow& window);
	void update(sf::Time dt);
	void draw();
	CommandQueue& getCommandQueue();
	void setRandomArray();
	void setArray(std::vector<int> data);
	void addToArray(int id, int value);
	void next();
	void previous();

private:
	void loadTextures();
	void buildScene();
	void addToArrayNext();
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
    std::vector<Aircraft*> mPlayerAircraftar, tmp_mPlayerAircraftar;
	CommandQueue mCommandQueue;
	int operationType = 0, totalSearchStep = 0, step = -1;
	std::pair <int, int> operation = {-1, -1};
};