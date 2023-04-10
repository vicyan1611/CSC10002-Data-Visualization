#pragma once

#include <SFML/Graphics.hpp>
#include <ResourceHolder.hpp>
#include <SceneNode.hpp>
#include <array>
#include <SpriteNode.hpp>
#include <CommandQueue.hpp>
#include <vector>
#include <LLNode.hpp>

class SllWorld: private sf::NonCopyable {
public:
	explicit SllWorld(sf::RenderWindow& window);
	void update(sf::Time dt);
	void draw();
	CommandQueue& getCommandQueue();
	void setArray(std::vector<int> data);
	void setRandomArray();
	void addToArray(int id, int value);
private:
	void loadTextures();
	void buildScene();
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

		std::vector<LLNode*> mSllNodes;
		sf::FloatRect mWorldBounds;
		CommandQueue mCommandQueue;

		int totalStep = 0, step = -1, operationType = 0;
		std::pair <int, int> operation = {-1, -1};
};