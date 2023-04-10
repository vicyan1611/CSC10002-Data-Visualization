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

	//test Node
	/*std::unique_ptr<LLNode> dllNode(new LLNode(1, mFonts, 15));
	dllNode->setPosition(700.f, 200.f);
	dllNode->setVelocity(0.f, 0.f);
	mSceneLayers[Air]->attachChild(std::move(dllNode));*/

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

void DllWorld::setArray(std::vector<int> data	) {
	if (!mDllNodes.empty()) {
		for (auto& node : mDllNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		mDllNodes.clear();
	}
	
	//create first nullptr
	std::unique_ptr<LLNode> fNull(new LLNode(0, mFonts, 0));
	fNull->setPosition(100.f, 100.f);
	fNull->setVelocity(0.f, 0.f);
	fNull->setString("nullptr");
	mDllNodes.push_back(fNull.get());
	mSceneLayers[Air]->attachChild(std::move(fNull));

	for (int i = 0; i < data.size(); ++i) {
		std::unique_ptr <LLNode> dllNode(new LLNode(data[i], mFonts, 3));
		dllNode->setPosition(100.f + (i + 1) * 180.f, 100.f);
		dllNode->setVelocity(0.f, 0.f);
		if (i == 0) dllNode->setColorSquare(sf::Color::Red);//not red, just an trash value
		mDllNodes.push_back(dllNode.get());
		mSceneLayers[Air]->attachChild(std::move(dllNode));
	}
}