#include <CllWorld.hpp>
#include <iostream>

CllWorld::CllWorld(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
{
	loadTextures();
	buildScene();
}

void CllWorld::loadTextures() {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	mTextures.load(Textures::Desert, "assets/GameState.jpg");
}

void CllWorld::buildScene() {
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

	//start node
	std::unique_ptr<LLNode> startNode(new LLNode(0, mFonts, 0));
	startNode->setPosition(100.f, 100.f);
	startNode->setVelocity(0.f, 0.f);
	mSceneLayers[Air]->attachChild(std::move(startNode));

	//test Node
	std::unique_ptr<LLNode> CllNode(new LLNode(1, mFonts, 8));
	CllNode->setPosition(500.f, 100.f);
	CllNode->setCll(true);
	CllNode->setVelocity(0.f, 0.f);
	mSceneLayers[Air]->attachChild(std::move(CllNode));
}

void CllWorld::update(sf::Time dt) {
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	mSceneGraph.update(dt);
}

void CllWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& CllWorld::getCommandQueue() {
	return mCommandQueue;
}