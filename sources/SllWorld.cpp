#include <SllWorld.hpp>
#include <iostream>

SllWorld::SllWorld(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
{
	loadTextures();
	buildScene();
}

void SllWorld::loadTextures() {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	mTextures.load(Textures::Desert, "assets/GameState.jpg");
}

void SllWorld::setArray(std::vector<int> data) {
	for (auto node : mSllNodes) {
		mSceneLayers[Air]->detachChild(*node);
	}
	mSllNodes.clear();
	
	//create pHead;
	std::unique_ptr<LLNode> pHead(new LLNode(0, mFonts, 1));
	pHead->setPosition(100.f, 100.f);
	pHead->setVelocity(0.f, 0.f);
	pHead->setString("pHead");
	mSllNodes.push_back(pHead.get());
	mSceneLayers[Air]->attachChild(std::move(pHead));
	
	for (int i = 0; i < data.size(); ++i) {
		std::unique_ptr<LLNode> sllNode(new LLNode(data[i], mFonts, 1));
		sllNode->setPosition(100.f + (i + 1) * 180.f, 100.f);
		sllNode->setVelocity(0.f, 0.f);
		mSllNodes.push_back(sllNode.get());
		mSceneLayers[Air]->attachChild(std::move(sllNode));
	}

	//create nullptr
	std::unique_ptr<LLNode> pNull(new LLNode(0, mFonts, 0));
	pNull->setPosition(100.f + (data.size() + 1) * 180.f, 100.f);
	pNull->setVelocity(0.f, 0.f);
	pNull->setString("nullptr");
	mSllNodes.push_back(pNull.get());
	mSceneLayers[Air]->attachChild(std::move(pNull));
}

void SllWorld::buildScene() {
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

    //test Node
	/*std::unique_ptr<LLNode> sllNode(new LLNode(1, mFonts, 2));
	sllNode->setPosition(700.f, 200.f);
	sllNode->setVelocity(0.f, 0.f);
	mSceneLayers[Air]->attachChild(std::move(sllNode));*/
}

void SllWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void SllWorld::update(sf::Time dt) {
	while (!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	mSceneGraph.update(dt);
}

CommandQueue& SllWorld::getCommandQueue() {
	return mCommandQueue;
}