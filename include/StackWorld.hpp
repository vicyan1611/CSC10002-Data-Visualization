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

class StackWorld : private sf::NonCopyable {
public:
	explicit StackWorld(sf::RenderWindow& window, int& id);
	void update(sf::Time dt, sf::Time& at);
	void draw();
	CommandQueue& getCommandQueue();

	void setArray(std::vector<int> data);
	void setRandomArray();
	void addToStack(int x);
	void next();
	void previous();
	void deleteFromStack();
	void deleteFromStackStep();
	void updateStack(int id, int x);
	void searchStack(int x);
	void runAtOnce();
	void loadFromFile();
	void clear();

private:
	void loadTextures(int id);
	void buildScene();
private:
	void addToStackStep();
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
	
	std::vector<LLNode*> mStackNodes, tmpNodes;
	Pseudocode* mPseudocode;

	sf::FloatRect mWorldBounds;
	CommandQueue mCommandQueue;

	int operationType = 0, value = 0, totalStep = 0, step = 0;
	bool isRunAtOnce = false;
};