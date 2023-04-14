#pragma once

#include <SFML/Graphics.hpp>
#include <ResourceHolder.hpp>
#include <SceneNode.hpp>
#include <array>
#include <SpriteNode.hpp>
#include <CommandQueue.hpp>
#include <vector>
#include <LLNode.hpp>

class QueueWorld : private sf::NonCopyable {
public:
	explicit QueueWorld(sf::RenderWindow& window);
	void update(sf::Time dt, sf::Time at);
	void draw();
	CommandQueue& getCommandQueue();

	void setArray(std::vector<int> data);
	void setRandomArray();
	void enqueue(int x);
	void dequeue();
	void next();
	void previous();
	void updateQueue(int id, int x);
	void searchQueue(int x);
	void runAtOnce();
private:
	void loadTextures();
	void buildScene();
	void enqueStep();
	void dequeueStep();
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

	std::vector <LLNode*> mQueueNodes, tmpNodes;

	sf::FloatRect mWorldBounds;
	CommandQueue mCommandQueue;

	int operationType = 0, value = 0, totalStep = 0, step = 0;
	bool isRunAtOnce = false;
};