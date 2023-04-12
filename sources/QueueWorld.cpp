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
		std::unique_ptr<LLNode> node(new LLNode(data[i], mFonts, (i == data.size() -1)? 0:1));
		node->setPosition(100.f + i * 180.f, 100.f);
		node->setVelocity(0.f, 0.f);
		if (i == 0) node->setColorSquare(sf::Color::Red);
		if (i == data.size() - 1) node->setColorSquare(sf::Color::Green);
		mQueueNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
	}
}

void QueueWorld::setRandomArray() {
	std::vector<int> data;
	int n = rand() % 8 + 1;
	for (int i = 0; i < n; ++i) {
		data.push_back(rand() % 100);
	}
	setArray(data);
}

void QueueWorld::enque(int x) {
	if (mQueueNodes.empty()) {
		std::vector <int> temp;
		temp.push_back(x);
		setArray(temp);
	}
	totalStep = 2;
	operationType = 1;
	step = 0;
	value = x;
}

void QueueWorld::enqueStep() {
	if (!tmpNodes.empty()) {
		for (auto& node : tmpNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpNodes.clear();
	}
	if (step == 0) return;
	if (step == 1) {
		std::unique_ptr<LLNode> tmpNode(new LLNode(value, mFonts, 0));
		tmpNode->setPosition(100.f + mQueueNodes.size() * 180.f, 100.f);
		tmpNode->setVelocity(0.f, 0.f);
		tmpNodes.push_back(tmpNode.get());
		mSceneLayers[Air]->attachChild(std::move(tmpNode));
	}
	else if (step == 2) {
		std::unique_ptr<LLNode> tmpNode(new LLNode(value, mFonts, 0));
		tmpNode->setPosition(100.f + mQueueNodes.size() * 180.f, 100.f);
		tmpNode->setVelocity(0.f, 0.f);
		tmpNode->setColorSquare(sf::Color::Green);
		mQueueNodes[mQueueNodes.size() - 1]->setColorSquare(sf::Color::White);
		mQueueNodes[mQueueNodes.size() - 1]->setDirection(1);
		if (mQueueNodes.size() == 1) tmpNode->setColorSquare(sf::Color::Red);
		mQueueNodes.push_back(tmpNode.get());
		mSceneLayers[Air]->attachChild(std::move(tmpNode));
	}
}

void QueueWorld::reUpdate() {
	totalStep = 0;
	operationType = 0;
	step = 0;
	value = 0;
	if (!tmpNodes.empty())
	{
		for (auto& node : tmpNodes)
		{
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpNodes.clear();
	}
}

void QueueWorld::next() {
	if (operationType == 0) return;
	step++;
	step = std::min(step, totalStep);

	if (operationType == 1) enqueStep();
	
	if (step >= totalStep) reUpdate();
}

void QueueWorld::previous() {
	if (operationType == 0) return;
	step--;
	step = std::max(step, 0);
	if (operationType == 1) enqueStep();

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