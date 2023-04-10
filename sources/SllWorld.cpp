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

void SllWorld::setRandomArray() {
	std::vector<int> data;
	int n = rand() % 8 + 1;
	for (int i = 0; i < n; ++i) {
		data.push_back(rand() % 100);
	}
	setArray(data);
}

void SllWorld::addToArray(int id, int value) {
	if (id < 1 || id > int(mSllNodes.size()) - 2) {
		std::cout << "Invalid id" << std::endl;
		return;
	}
	operationType = 1;
	totalStep = (id - 1) + 1 + 1;
	step = 0;
	operation = {id, value};
	for (int i = 0; i < mSllNodes.size(); ++i) {
		mSceneLayers[Air]->detachChild(*mSllNodes[i]);
	}
	std::cout << id << " " << value << std::endl;
}

void SllWorld::addToArrayStep() {
	if (!tmpSllNodes.empty()) {
		for (auto node : tmpSllNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpSllNodes.clear();
	}
	if (step == 0) return;
	int tmpStep = step;
	for (int i = 0; i < mSllNodes.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(1, mFonts, 1));
		node->setPosition(mSllNodes[i]->getPosition());
		node->setVelocity(0.f, 0.f);
		if (i == 0) {
			node->setString("pHead");
		}
		else if (i == mSllNodes.size() - 1) {
			node->setString("nullptr");
		}
		else {
			node->setString(std::to_string(mSllNodes[i]->getValue()));
		}
		tmpSllNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
	}
	if (tmpStep < operation.first) {
		tmpSllNodes[tmpStep]->setColor(sf::Color::Cyan);
	}
	else if (tmpStep == operation.first) {
		tmpSllNodes[tmpStep]->setColor(sf::Color::Cyan);
		std::unique_ptr<LLNode> newNode(new LLNode(operation.second, mFonts, 1));
		newNode->setPosition(tmpSllNodes[tmpStep]->getPosition() + sf::Vector2f(0.f, 50.f));
		newNode->setVelocity(0.f, 0.f);
		tmpSllNodes.insert(tmpSllNodes.begin() + tmpStep, newNode.get());
		mSceneLayers[Air]->attachChild(std::move(newNode));
	}
	else {
		for (int i = 0; i < tmpSllNodes.size(); ++i) {
			tmpSllNodes[i]->setPosition(100.f + (i + 1) * 180.f, 100.f);
		}
	}
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