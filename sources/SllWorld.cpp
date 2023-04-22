#include <SllWorld.hpp>
#include <iostream>
#include <fstream>
SllWorld::SllWorld(sf::RenderWindow& window, int& id)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
{
	loadTextures(id);
	buildScene();
}

void SllWorld::loadTextures(int id) {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	if (id == 0) mTextures.load(Textures::Desert, "assets/TitleScreen.png");
	if (id == 1) mTextures.load(Textures::Desert, "assets/TitleScreen1.jpg");
	if (id == 2) mTextures.load(Textures::Desert, "assets/TitleScreen2.jpg");
	if (id == 3) mTextures.load(Textures::Desert, "assets/TitleScreen3.jpg");
	if (id == 4) mTextures.load(Textures::Desert, "assets/TitleScreen4.jpg");
	if (id == 5) mTextures.load(Textures::Desert, "assets/TitleScreen5.jpg");
}

void SllWorld::setArray(std::vector<int> data) {
	for (auto node : mSllNodes) {
		mSceneLayers[Air]->detachChild(*node);
	}
	mSllNodes.clear();
	
	for (int i = 0; i < data.size(); ++i) {
		std::unique_ptr<LLNode> sllNode(new LLNode(data[i], mFonts, (i == data.size() - 1) ? 0 : 1));
		sllNode->setPosition(100.f + i * 180.f, 100.f);
		sllNode->setVelocity(0.f, 0.f);
		if (i == 0) sllNode->setColorSquare(sf::Color::Red);
		if (i == data.size() - 1) sllNode->setColorSquare(sf::Color::Green);
		mSllNodes.push_back(sllNode.get());
		mSceneLayers[Air]->attachChild(std::move(sllNode));
	}
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
	mValue.clear();
	for (int i = 0; i < mSllNodes.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(1, mFonts, (i == mSllNodes.size() - 1) ? 0: 1));
		node->setPosition(mSllNodes[i]->getPosition());
		node->setVelocity(0.f, 0.f);
		node->setValue(mSllNodes[i]->getValue());
		mValue.push_back(mSllNodes[i]->getValue());
		tmpSllNodes.push_back(node.get());
		if (i == 0) node->setColorSquare(sf::Color::Red);
		if (i == mSllNodes.size() - 1) node->setColorSquare(sf::Color::Green);
		mSceneLayers[Air]->attachChild(std::move(node));
		mSceneLayers[Air]->detachChild(*mSllNodes[i]);
	}
	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 4));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void SllWorld::addToArrayStep() {
	if (step == 0) return;
	if (!tmpSllNodes.empty()) {
		for (auto& node : tmpSllNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpSllNodes.clear();
	}
	mPseudocode->resetColor();
	int tmpStep = step;
	for (int i = 0; i < mSllNodes.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(1, mFonts, (i == mSllNodes.size() - 1) ? 0 : 1));
		node->setPosition(100.f + i * 180.f, 100.f);
		node->setVelocity(0.f, 0.f);
		if (i == 0) node->setColorSquare(sf::Color::Red);
		if (i == mSllNodes.size() - 1) node->setColorSquare(sf::Color::Green);
		node->setValue(mValue[i]);
		tmpSllNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
	}
	if (tmpStep < operation.first) {
		int tmpID = tmpStep-1;
		if (tmpID == 0) mPseudocode->setColorText(1); else mPseudocode->setColorText(2);
		tmpSllNodes[tmpID]->setColor(sf::Color::Cyan);
	}
	else if (tmpStep >= operation.first) {
		int tmpID = tmpStep - 1;
		tmpSllNodes[tmpID]->setColor(sf::Color::Cyan);
		std::unique_ptr<LLNode> newNode(new LLNode(operation.second, mFonts, 4));
		newNode->setPosition(tmpSllNodes[tmpID]->getPosition() + sf::Vector2f(0.f, 130.f));
		newNode->setVelocity(0.f, 0.f);
		tmpSllNodes.insert(tmpSllNodes.begin() + (operation.first-1), newNode.get());
		mSceneLayers[Air]->attachChild(std::move(newNode));
		mPseudocode->setColorText(3);
		mPseudocode->setColorText(4);
	}
	if (tmpStep > operation.first) {
		mPseudocode->resetColor();
		tmpSllNodes[operation.first - 1]->setDirection(1);
		mPseudocode->setColorText(5);
		for (int i = 0; i < tmpSllNodes.size(); ++i) {
			tmpSllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
			tmpSllNodes[i]->setColor(sf::Color::White);
		}
	}
}

void SllWorld::deleteFromArray(int id) {
	if (id < 1 || id > int(mSllNodes.size()) - 2) return;
	operationType = 2;
	totalStep = id + 1;
	operation = { id, -1 };
	step = 0;
	mValue.clear();
	for (int i = 0; i < mSllNodes.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(1, mFonts, (i == mSllNodes.size() - 1) ? 0 : 1));
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
			mValue.push_back(mSllNodes[i]->getValue());
		}
		tmpSllNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
		mSceneLayers[Air]->detachChild(*mSllNodes[i]);
	}
	std::cout << id << "\n";
}

void SllWorld::deleteFromArrayStep() {
	if (step == 0) return;
	if (!tmpSllNodes.empty()) {
		for (auto node : tmpSllNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpSllNodes.clear();
	}
	for (int i = 0; i < mSllNodes.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(1, mFonts, (i == mSllNodes.size() - 1) ? 0 : 1));
		node->setPosition(100.f + i * 180.f, 100.f);
		node->setVelocity(0.f, 0.f);
		if (i == 0) {
			node->setString("pHead");
		}
		else if (i == mSllNodes.size() - 1) {
			node->setString("nullptr");
		}
		else {
			int tmpID = i - 1;
			node->setValue(mValue[tmpID]);
		}
		tmpSllNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
	}
	if (step <= operation.first) {
		tmpSllNodes[step]->setColor(sf::Color::Cyan);
	}
	else {
		mSceneLayers[Air]->detachChild(*tmpSllNodes[operation.first]);
		tmpSllNodes.erase(tmpSllNodes.begin() + operation.first);
		for (int i = 0; i < tmpSllNodes.size(); ++i) {
			tmpSllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
			tmpSllNodes[i]->setColor(sf::Color::White);
		}
	}
}

void SllWorld::updateArrayStep() {
	if (step == 0) return;
	for (auto node : mSllNodes)
		node->setColor(sf::Color::White);
	if (step <= operation.first)
		mSllNodes[step]->setColor(sf::Color::Cyan);
	else {
		mSllNodes[operation.first]->setValue(operation.second);
	}
}

void SllWorld::updateArray(int id, int value) {
	if (id < 1 || id > int(mSllNodes.size()) - 2) {
		std::cout << "Invalid id" << std::endl;
		return;
	}
	operationType = 3;
	step = 0;
	totalStep = id + 1;
	operation = { id, value };
}

void SllWorld::searchArrayStep() {
	if (step == 0) return;
	for (int i = 0; i < mSllNodes.size(); ++i) {
		mSllNodes[i]->setColor(sf::Color::White);
	}
	int tmpStep = step;
	for (int i = 1; i < mSllNodes.size() - 1; ++i) {
		if (mSllNodes[i]->getValue() == operation.second) {
			mSllNodes[i]->setColor(sf::Color::Cyan);
		}
		tmpStep--;
		if (tmpStep == 0) {
			mSllNodes[i]->setColor(sf::Color::Green);
			break;
		}
	}
	if (tmpStep == 0) return;
	if (mSllNodes[mSllNodes.size() - 2]->getValue() == operation.second) {
		mSllNodes[mSllNodes.size() - 2]->setColor(sf::Color::Cyan);
		tmpStep--;
	}
	if (tmpStep == 0) return;
	for (int i = 0; i < mSllNodes.size(); ++i) {
		mSllNodes[i]->setColor(sf::Color::White);
	}
}

void SllWorld::searchArray(int value) {
	if (mSllNodes.empty()) return;
	operationType = 4;
	step = 0;
	totalStep = int(mSllNodes.size()) - 2 + 1;
	int tmpID = int(mSllNodes.size()) - 2;
	if (mSllNodes[tmpID]->getValue() == value) {
		totalStep += 1;
	}
	operation = { -1, value };
}

void SllWorld::reUpdate() {
	isRunAtOnce = false;
	if (mPseudocode != nullptr) {
		mSceneLayers[Air]->detachChild(*mPseudocode);
	}
	if (operationType == 3 || operationType == 4) {
		operationType = 0;
		step = totalStep = 0;
		return;
	}
	operationType = 0;
	step = totalStep = 0;
	mSllNodes.clear();
	for (int i = 0; i < tmpSllNodes.size(); ++i) {
		mSllNodes.push_back(tmpSllNodes[i]);
	}
	tmpSllNodes.clear();
	operation = { -1, -1 };
}

void SllWorld::next() {
	if (operationType == 0) return;
	
	step++;
	step = std::min(step, totalStep);

	if (operationType == 1) addToArrayStep();
	else if (operationType == 2) deleteFromArrayStep();
	else if (operationType == 3) updateArrayStep();
	else if (operationType == 4) searchArrayStep();

	if (step >= totalStep) reUpdate();
}

void SllWorld::previous() {
	if (operationType == 0) return;
	step--;
	step = std::max(step, 0);
	if (operationType == 1) addToArrayStep();
	else if (operationType == 2) deleteFromArrayStep();
	else if (operationType == 3) updateArrayStep();
	else if (operationType == 4) searchArrayStep();
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

void SllWorld::runAtOnce() {
	isRunAtOnce = true;
}

void SllWorld::update(sf::Time dt, sf::Time& at) {
	while (!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	mSceneGraph.update(dt, at);
	if (at >= sf::seconds(1.f) && isRunAtOnce) {
		at = sf::Time::Zero;
		next();
	}
}

void SllWorld::loadFromFile() {
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

CommandQueue& SllWorld::getCommandQueue() {
	return mCommandQueue;
}