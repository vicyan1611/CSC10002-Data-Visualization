#include <QueueWorld.hpp>
#include <iostream>

QueueWorld::QueueWorld(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
{
	loadTextures();
	buildScene();
}

void QueueWorld::loadTextures() {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	mTextures.load(Textures::Desert, "assets/GameState.jpg");
}

void QueueWorld::buildScene() {
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

void QueueWorld::setArray(std::vector<int> data) {
	if (!mQueueNodes.empty()) {
		for (auto& node : mQueueNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		mQueueNodes.clear();
	}
	for (int i = 0; i < data.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(data[i], mFonts, 1));
		node->setPosition(100.f + i * 180.f, 100.f);
		node->setVelocity(0.f, 0.f);
		if (i == 0 || i == data.size() - 1) node->setColorSquare(sf::Color::Red);
		mQueueNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
	}
	//create last nullptr
	std::unique_ptr<LLNode> node(new LLNode(-1, mFonts, 0));
	node->setPosition(100.f + data.size() * 180.f, 100.f);
	node->setVelocity(0.f, 0.f);
	node->setString("nullptr");
	mQueueNodes.push_back(node.get());
	mSceneLayers[Air]->attachChild(std::move(node));
}

void QueueWorld::update(sf::Time dt) {
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	mSceneGraph.update(dt);
}

void QueueWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& QueueWorld::getCommandQueue() {
	return mCommandQueue;
}