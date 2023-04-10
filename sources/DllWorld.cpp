#include <DllWorld.hpp>
#include <iostream>

DllWorld::DllWorld(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
{
	loadTextures();
	buildScene();
}

void DllWorld::loadTextures() {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	mTextures.load(Textures::Desert, "assets/GameState.jpg");
}

void DllWorld::buildScene() {
	for (std::size_t i = 0; i < LayerCount; ++i) {
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	//add background
	sf::Texture& texture = mTextures.get(Textures::Desert);
	sf::IntRect textureRect(mWorldBounds);
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));
}

void DllWorld::update(sf::Time dt) {
	while (!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	mSceneGraph.update(dt);
}

void DllWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& DllWorld::getCommandQueue() {
	return mCommandQueue;
}