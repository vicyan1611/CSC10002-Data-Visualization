#pragma once

#include <SFML/Graphics.hpp>
#include <ResourceHolder.hpp>
#include <SceneNode.hpp>
#include <array>
#include <SpriteNode.hpp>
#include <CommandQueue.hpp>
#include <vector>

class SllWorld: private sf::NonCopyable {
public:
	explicit SllWorld(sf::RenderWindow& window);
	void update(sf::Time dt);
	void draw();
	CommandQueue& getCommandQueue();
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

		sf::FloatRect mWorldBounds;
		CommandQueue mCommandQueue;
};