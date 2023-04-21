#pragma once

#include <SFML/Graphics.hpp>
#include <ResourceHolder.hpp>
#include <SceneNode.hpp>
#include <Aircraft.hpp>
#include <array>
#include <SpriteNode.hpp>
#include <CommandQueue.hpp>
#include <vector>
#include <Pseudocode.hpp>

class DynamicArrayWorld : private sf::NonCopyable {
public:
	explicit DynamicArrayWorld(sf::RenderWindow& window, int& id);
	void update(sf::Time dt, sf::Time& at);
	void draw();
	CommandQueue& getCommandQueue();
	void setRandomArray();
	void setArray(std::vector<int> data);
	void addToArray(int id, int value);
	void updateArray(int id, int value);
	void deleteFromArray(int id);
	void searchArray(int value);
	void next();
	void previous();
	void runAtOnce();
	void loadFromFile();
private:
	void loadTextures(int id);
	void buildScene();
	void addToArrayStep();
	void deleteFromArrayStep();
	void searchArrayStep();
	void reUpdate();
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
	Pseudocode* mPseudocode = nullptr;
	CommandQueue mCommandQueue;
	
	int operationType = 0, totalSearchStep = 0, step = -1;
	std::pair <int, int> operation = {-1, -1};
	bool isRunAtOnce = false;
};