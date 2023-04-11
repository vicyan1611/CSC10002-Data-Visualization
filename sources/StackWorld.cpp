#include <StackWorld.hpp>
#include <iostream>

StackWorld::StackWorld(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
{
	loadTextures();
	buildScene();
}

void StackWorld::loadTextures() {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	mTextures.load(Textures::Desert, "assets/GameState.jpg");
}

void StackWorld::buildScene() {
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

void StackWorld::update(sf::Time dt) {
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	mSceneGraph.update(dt);
}

void StackWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& StackWorld::getCommandQueue() {
	return mCommandQueue;
}

void StackWorld::setArray(std::vector<int> data) {
	for (int i = 0; i < data.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(data[i], mFonts, 1));
		node->setPosition(100.f + i * 180.f, 100.f);
		node->setVelocity(0.f, 0.f);
		if (i == 0) node->setColorSquare(sf::Color::Red); //not a real value
		mStackNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
	}
	//create last nullptr;
	std::unique_ptr<LLNode> lnull(new LLNode(0, mFonts, 0));
	lnull->setPosition(100.f + data.size() * 180.f, 100.f);
	lnull->setVelocity(0.f, 0.f);
	lnull->setString("nullptr");
	mStackNodes.push_back(lnull.get());
	mSceneLayers[Air]->attachChild(std::move(lnull));
}