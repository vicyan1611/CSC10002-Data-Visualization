#include <QueueWorld.hpp>
#include <iostream>
#include <fstream>

QueueWorld::QueueWorld(sf::RenderWindow& window, int& id)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
{
	loadTextures(id);
	buildScene();
}

void QueueWorld::loadTextures(int id) {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	if (id == 0) mTextures.load(Textures::Desert, "assets/TitleScreen.png");
	if (id == 1) mTextures.load(Textures::Desert, "assets/TitleScreen1.jpg");
	if (id == 2) mTextures.load(Textures::Desert, "assets/TitleScreen2.jpg");
	if (id == 3) mTextures.load(Textures::Desert, "assets/TitleScreen3.jpg");
	if (id == 4) mTextures.load(Textures::Desert, "assets/TitleScreen4.jpg");
	if (id == 5) mTextures.load(Textures::Desert, "assets/TitleScreen5.jpg");
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
	if (data.size() == 0) return;
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

void QueueWorld::enqueue(int x) {
	if (mQueueNodes.empty()) {
		std::vector <int> temp;
		temp.push_back(x);
		setArray(temp);
	}
	totalStep = 3;
	operationType = 1;
	step = 0;
	value = x;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 12));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void QueueWorld::dequeue() {
	if (mQueueNodes.empty()) return;
	totalStep = 3;
	operationType = 2;
	step = 0;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 11));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void QueueWorld::searchQueue(int x) {
	if (mQueueNodes.empty()) return;
	for (int i = 0; i < mQueueNodes.size(); ++i) {
		mQueueNodes[i]->setColor(sf::Color::White);
		if (mQueueNodes[i]->getValue() == x) {
			mQueueNodes[i]->setColor(sf::Color::Cyan);
		}
	}
}

void QueueWorld::enqueStep() {
	if (!tmpNodes.empty()) {
		for (auto& node : tmpNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpNodes.clear();
	}
	mPseudocode->resetColor();
	if (step == 0) return;
	if (step == 1) {
		std::unique_ptr<LLNode> tmpNode(new LLNode(value, mFonts, 0));
		tmpNode->setPosition(100.f + mQueueNodes.size() * 180.f, 100.f);
		tmpNode->setVelocity(0.f, 0.f);
		tmpNodes.push_back(tmpNode.get());
		mSceneLayers[Air]->attachChild(std::move(tmpNode));
		mPseudocode->setColorText(1);
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
		mPseudocode->setColorText(2); mPseudocode->setColorText(3);
	}
}

void QueueWorld::dequeueStep() {
	for (int i = 0; i < mQueueNodes.size(); ++i) {
		if (i == 0) mQueueNodes[i]->setColorSquare(sf::Color::Red); 
		else if (i == mQueueNodes.size() - 1) mQueueNodes[i]->setColorSquare(sf::Color::Green);
		else mQueueNodes[i]->setColorSquare(sf::Color::White);
	}
	mPseudocode->resetColor();
	if (step == 0) return;
	if (step == 1) {
		mQueueNodes[0]->setColorSquare(sf::Color::White);
		if (mQueueNodes.size() > 1) mQueueNodes[1]->setColorSquare(sf::Color::Red);
		mPseudocode->setColorText(1);
		mPseudocode->setColorText(2);
	}
	else
	{
		if (mQueueNodes.size() == 1)
		{
			mSceneLayers[Air]->detachChild(*mQueueNodes[0]);
			mQueueNodes.clear();
		}
		else {
			mSceneLayers[Air]->detachChild(*mQueueNodes[0]);
			mQueueNodes.erase(mQueueNodes.begin());
			if (mQueueNodes.size() > 1) mQueueNodes[0]->setColorSquare(sf::Color::Red);
		}
		for (int i = 0; i < mQueueNodes.size(); ++i) {
			mQueueNodes[i]->setPosition(100.f + i * 180.f, 100.f);
		}
		mPseudocode->setColorText(3);
	}
}

void QueueWorld::updateQueue(int id, int x) {
	if (id < 1 || id > mQueueNodes.size()) return;
	int tmpID = id - 1;
	mQueueNodes[tmpID]->setValue(x);
}

void QueueWorld::reUpdate() {
	totalStep = 0;
	operationType = 0;
	step = 0;
	value = 0;
	isRunAtOnce = false;
	if (!tmpNodes.empty())
	{
		for (auto& node : tmpNodes)
		{
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpNodes.clear();
	}
	if (mPseudocode != nullptr) {
		mSceneLayers[Air]->detachChild(*mPseudocode);
		mPseudocode = nullptr;
	}
}

void QueueWorld::next() {
	if (operationType == 0) return;
	step++;
	step = std::min(step, totalStep);

	if (operationType == 1) enqueStep();
	else if (operationType == 2) dequeueStep();
	
	if (step >= totalStep) reUpdate();
}

void QueueWorld::previous() {
	if (operationType == 0) return;
	step--;
	step = std::max(step, 0);
	if (operationType == 1) enqueStep();
	else if (operationType == 2) dequeueStep();

}

void QueueWorld::runAtOnce() {
	isRunAtOnce = true;
}

void QueueWorld::update(sf::Time dt, sf::Time& at) {
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	mSceneGraph.update(dt, at);
	if (at >= sf::seconds(1.f) && isRunAtOnce) {
		at = sf::Time::Zero;
		next();
	}
}

void QueueWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& QueueWorld::getCommandQueue() {
	return mCommandQueue;
}

void QueueWorld::loadFromFile() {
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
	file.close();
}

void QueueWorld::clear() {
	std::vector <int> v;
	v.clear();
	setArray(v);
}