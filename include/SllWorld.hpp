#pragma once

#include <SFML/Graphics.hpp>
#include <ResourceHolder.hpp>
#include <SceneNode.hpp>
#include <array>
#include <SpriteNode.hpp>
#include <CommandQueue.hpp>
#include <vector>
#include <LLNode.hpp>
#include <Pseudocode.hpp>

class SllWorld : private sf::NonCopyable {
public:
	explicit SllWorld(sf::RenderWindow& window, int& id);
	void update(sf::Time dt, sf::Time& at);
	void draw();
	CommandQueue& getCommandQueue();
	void setArray(std::vector<int> data);
	void setRandomArray();
	void addToArray(int id, int value);
	void next();
	void previous();
	void deleteFromArray(int id);
	void updateArray(int id, int value);
	void searchArray(int value);
	void runAtOnce();
	void loadFromFile();
private:
	void loadTextures(int id);
	void buildScene();
	void addToArrayStep();
	void deleteFromArrayStep();
	void updateArrayStep();
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

	std::vector<LLNode*> mSllNodes, tmpSllNodes;
	sf::FloatRect mWorldBounds;
	CommandQueue mCommandQueue;
	Pseudocode* mPseudocode;

	int totalStep = 0, step = -1, operationType = 0;
	std::pair <int, int> operation = { -1, -1 };
	std::vector<int> mValue;
	bool isRunAtOnce = false;
};