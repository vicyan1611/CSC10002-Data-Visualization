#include <StackWorld.hpp>
#include <iostream>
#include <fstream>

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

void StackWorld::update(sf::Time dt, sf::Time& at) {
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	mSceneGraph.update(dt, at);
	if (at >= sf::seconds(1.f) && isRunAtOnce) {
		at = sf::Time::Zero;
		next();
	}
}

void StackWorld::runAtOnce() {
	isRunAtOnce = true;
}

void StackWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& StackWorld::getCommandQueue() {
	return mCommandQueue;
}

void StackWorld::setArray(std::vector<int> data) {
	if (!mStackNodes.empty()) {
		for (auto& node : mStackNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		mStackNodes.clear();
	}
	for (int i = 0; i < data.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(data[i], mFonts, 1));
		node->setPosition(100.f + i * 180.f, 100.f);
		node->setVelocity(0.f, 0.f);
		if (i == 0) node->setColorSquare(sf::Color::Red);
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

void StackWorld::setRandomArray() {
	std::vector<int> data;
	int n = rand() % 8 + 1;
	for (int i = 0; i < n; ++i) {
		data.push_back(rand() % 100);
	}
	setArray(data);
}

void StackWorld::addToStack(int x) {
	if (mStackNodes.empty()) {
		std::vector <int> a;
		a.push_back(x);
		setArray(a);
		return;
	}
	operationType = 1;
	totalStep = 3;
	step = 0;
	value = x;
}

void StackWorld::addToStackStep() {
	if (!tmpNodes.empty()) {
		for (auto& node : tmpNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpNodes.clear();
	}
	mStackNodes[0]->setColorSquare(sf::Color::Red);

	if (step == 0) return;
	std::cout << "step1: " << step << std::endl;
	int tmpStep = step;
	std::unique_ptr<LLNode> tmpNode(new LLNode(value, mFonts, 4));
	tmpNode->setPosition(100.f, 230.f);
	tmpNode->setVelocity(0.f, 0.f);
	tmpNodes.push_back(tmpNode.get());
	mSceneLayers[Air]->attachChild(std::move(tmpNode));
	

	if (--tmpStep == 0) return;
	std::cout << "step2: " << tmpStep << std::endl;
	mStackNodes[0]->setColorSquare(sf::Color::White);
	tmpNodes[0]->setColorSquare(sf::Color::Red);

	if (--tmpStep == 0) return;

	std::unique_ptr<LLNode> node(new LLNode(value, mFonts, 1));
	node->setVelocity(0.f, 0.f);
	mStackNodes.insert(mStackNodes.begin(), node.get());
	mSceneLayers[Air]->attachChild(std::move(node));

	for (int i = 0; i < mStackNodes.size(); ++i) {
		mStackNodes[i]->setPosition(100.f + i * 180.f, 100.f);
		mStackNodes[i]->setColorSquare(sf::Color::White);
		if (i == 0) mStackNodes[i]->setColorSquare(sf::Color::Red);
	}
}

void StackWorld::deleteFromStack() {
	if (mStackNodes.empty()) return;
	operationType = 2;
	totalStep = 2;
	step = 0;
}

void StackWorld::deleteFromStackStep() {
	if (step == 1) {
		mStackNodes[0]->setColorSquare(sf::Color::White);
		mStackNodes[1]->setColorSquare(sf::Color::Red);
	}
	if (step == 2) {
		mSceneLayers[Air]->detachChild(*mStackNodes[0]);
		mStackNodes.erase(mStackNodes.begin());
		for (int i = 0; i < mStackNodes.size(); ++i) {
			mStackNodes[i]->setPosition(100.f + i * 180.f, 100.f);
		}
	}
}

void StackWorld::updateStack(int id, int x) {
	if (id < 0 || id >= mStackNodes.size() - 1) return;
	int tmpID = id - 1;
	mStackNodes[tmpID]->setValue(x);
}

void StackWorld::searchStack(int x) {
	if (mStackNodes.empty()) return;
	for (int i = 0; i < mStackNodes.size() - 1; ++i) {
		mStackNodes[i]->setColor(sf::Color::White);
		if (mStackNodes[i]->getValue() == x) {
			mStackNodes[i]->setColor(sf::Color::Cyan);
		}
	}
}

void StackWorld::reUpdate() {
	operationType = 0;
	step = 0;
	totalStep = 0;
	value = 0;
	isRunAtOnce = false;
	if (!tmpNodes.empty()) {
		for (auto& node : tmpNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpNodes.clear();
	}
}

void StackWorld::next() {
	if (operationType == 0) return;
	step++;
	step = std::min(step, totalStep);

	if (operationType == 1) addToStackStep();
	else if (operationType == 2) deleteFromStackStep();

	if (step >= totalStep) reUpdate();
}

void StackWorld::previous() {
	if (operationType == 0) return;
	step--;
	step = std::max(step, 0);
	if (operationType == 1) addToStackStep();
	else if (operationType == 2) deleteFromStackStep();
}

void StackWorld::loadFromFile() {
	std::ifstream file("data.txt");
	std::vector<int> data;
	int n;
	file >> n;
	for (int i = 0; i < n; ++i) {
		int x;
		file >> x;
		data.push_back(x);
	}
	setArray(data);
}