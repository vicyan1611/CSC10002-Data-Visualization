#include <DllWorld.hpp>
#include <iostream>
#include <fstream>

DllWorld::DllWorld(sf::RenderWindow& window, int& id)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
{
	loadTextures(id);
	buildScene();
}

void DllWorld::loadTextures(int id) {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	if (id == 0) mTextures.load(Textures::Desert, "assets/TitleScreen.png");
	if (id == 1) mTextures.load(Textures::Desert, "assets/TitleScreen1.jpg");
	if (id == 2) mTextures.load(Textures::Desert, "assets/TitleScreen2.jpg");
	if (id == 3) mTextures.load(Textures::Desert, "assets/TitleScreen3.jpg");
	if (id == 4) mTextures.load(Textures::Desert, "assets/TitleScreen4.jpg");
	if (id == 5) mTextures.load(Textures::Desert, "assets/TitleScreen5.jpg");
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

void DllWorld::update(sf::Time dt, sf::Time& at) {
	while (!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	mSceneGraph.update(dt, at);
	if (at >= sf::seconds(1.f) && isRunAtOnce) {
		at = sf::Time::Zero;
		next();
	}
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
	for (int i = 0; i < data.size(); ++i) {
		std::unique_ptr <LLNode> dllNode(new LLNode(data[i], mFonts, 3));
		dllNode->setPosition(100.f + i * 180.f, 100.f);
		dllNode->setVelocity(0.f, 0.f);
		if (i == 0)
		{
			dllNode->setColorSquare(sf::Color::Red);
			dllNode->setDirection(1);
		}
		if (i == data.size() - 1)
		{
			dllNode->setColorSquare(sf::Color::Green);
			dllNode->setDirection(2);
		}
		mDllNodes.push_back(dllNode.get());
		mSceneLayers[Air]->attachChild(std::move(dllNode));
	}
	if (data.size() == 1) mDllNodes[0]->setDirection(0);
}

void DllWorld::setRandomArray() {
	std::vector<int> data;
	int n = rand() % 8 + 1;
	for (int i = 0; i < n; ++i) {
		data.push_back(rand() % 100);
	}
	setArray(data);
}

void DllWorld::addToArray(int id, int value) {
	if (id < 1 || id > mDllNodes.size() + 1) {
		std::cout << "Invalid id" << std::endl;
		return;
	}
	if (operationType != 0) return;
	operationType = 1;
	totalStep = id + 2;
	step = 0;
	operation = { id, value };
	mValue.clear();
	for (int i = 0; i < mDllNodes.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(1, mFonts, 3));
		node->setPosition(mDllNodes[i]->getPosition());
		node->setVelocity(0.f, 0.f);
		node->setValue(mDllNodes[i]->getValue());
		mValue.push_back(mDllNodes[i]->getValue());
		if (i == 0)
		{
			node->setColorSquare(sf::Color::Red);
			node->setDirection(1);
		}
		if (i == mDllNodes.size() - 1)
		{
			node->setColorSquare(sf::Color::Green);
			node->setDirection(2);
		}
		tmpDllNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
		mSceneLayers[Air]->detachChild(*mDllNodes[i]);
	}
	if (tmpDllNodes.size() == 1) mDllNodes[0]->setDirection(0);
	std::cout << id << " add to DLL " << value << std::endl;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 8));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void DllWorld::deleteFromArray(int id) {
	if (id < 1 || id > mDllNodes.size()) {
		std::cout << "Invalid id" << std::endl;
		return;
	}
	operationType = 2;
	totalStep = id + 1;
	step = 0;
	operation = { id, 0 };
	mValue.clear();
	for (int i = 0; i < mDllNodes.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(1, mFonts, 3));
		node->setPosition(mDllNodes[i]->getPosition());
		node->setVelocity(0.f, 0.f);
		node->setValue(mDllNodes[i]->getValue());
		mValue.push_back(mDllNodes[i]->getValue());
		if (i == 0)
		{
			node->setColorSquare(sf::Color::Red);
			node->setDirection(1);
		}
		if (i == mDllNodes.size() - 1)
		{
			node->setColorSquare(sf::Color::Green);
			node->setDirection(2);
		}
		tmpDllNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
		mSceneLayers[Air]->detachChild(*mDllNodes[i]);
	}
	if (tmpDllNodes.size() == 1) mDllNodes[0]->setDirection(0);

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 9));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void DllWorld::updateArray(int id, int value) {
	if (id < 1 || id > int(mDllNodes.size()) ) {
		std::cout << "Invalid id" << std::endl;
		return;
	}
	operationType = 3;
	totalStep = id + 2;
	step = 0;
	operation = { id, value };

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 6));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void DllWorld::searchArray(int value) {
	if (mDllNodes.empty()) return;
	operationType = 4;
	totalStep = mDllNodes.size() + 2;
	step = 0;
	operation = { 0, value };
	int tmpID = mDllNodes.size() - 1;
	if (mDllNodes[tmpID]->getValue() == value) totalStep++;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 7));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void DllWorld::addToArrayStep() {
	if (step == 0) return;
	if (!tmpDllNodes.empty()) {
		for (auto& node : tmpDllNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpDllNodes.clear();
	}
	mPseudocode->resetColor();
	int tmpStep = step;
	for (int i = 0; i < mDllNodes.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(1, mFonts, 3));
		node->setPosition(100.f + i * 180.f, 100.f);
		node->setVelocity(0.f, 0.f);
		node->setValue(mValue[i]);
		if (i == 0)
		{
			node->setColorSquare(sf::Color::Red);
			node->setDirection(1);
		}
		if (i == mDllNodes.size() - 1)
		{
			node->setColorSquare(sf::Color::Green);
			node->setDirection(2);
		}
		tmpDllNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
	}
	if (tmpDllNodes.size() == 1) mDllNodes[0]->setDirection(0);
	if (tmpStep < operation.first) {
		tmpDllNodes[tmpStep-1]->setColor(sf::Color::Cyan);
		if (tmpStep == 1) mPseudocode->setColorText(1);
		else mPseudocode->setColorText(2);
	}
	else if (tmpStep >= operation.first) {
		if (operation.first == tmpDllNodes.size() + 1) {
			mPseudocode->setColorText(3);
			std::unique_ptr<LLNode> newNode(new LLNode(operation.second, mFonts, 3));
			newNode->setPosition(tmpDllNodes[tmpDllNodes.size() - 1]->getPosition() + sf::Vector2f(180.f, 0.f));
			newNode->setVelocity(0.f, 0.f);
			tmpDllNodes.push_back(newNode.get());
			mSceneLayers[Air]->attachChild(std::move(newNode));
		}
		else {
			tmpDllNodes[operation.first - 1]->setColor(sf::Color::Cyan);
			mPseudocode->setColorText(3);
			std::unique_ptr<LLNode> newNode(new LLNode(operation.second, mFonts, 3));
			newNode->setPosition(tmpDllNodes[operation.first - 1]->getPosition() + sf::Vector2f(0.f, 75.f));
			newNode->setVelocity(0.f, 0.f);
			tmpDllNodes.insert(tmpDllNodes.begin() + operation.first - 1, newNode.get());
			mSceneLayers[Air]->attachChild(std::move(newNode));
		}
		
	}
	if (tmpStep > operation.first) {
		for (int i = 0; i < tmpDllNodes.size(); ++i) {
			tmpDllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
			tmpDllNodes[i]->setColor(sf::Color::White);
			tmpDllNodes[i]->setDirection(3);
			tmpDllNodes[i]->setColorSquare(sf::Color::White);
			if (i == 0)
			{
				tmpDllNodes[i]->setColorSquare(sf::Color::Red);
				tmpDllNodes[i]->setDirection(1);
			} 
			if (i == tmpDllNodes.size() - 1) {
				tmpDllNodes[i]->setColorSquare(sf::Color::Green);
				tmpDllNodes[i]->setDirection(2);
			}
		}
		mPseudocode->resetColor(); mPseudocode->setColorText(4); mPseudocode->setColorText(5);
	}
}

void DllWorld::deleteFromArrayStep() {
	if (step == 0) return;
	if (!tmpDllNodes.empty()) {
		for (auto& node : tmpDllNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpDllNodes.clear();
	}
	mPseudocode->resetColor();
	int tmpStep = step;
	for (int i = 0; i < mDllNodes.size(); ++i) {
		std::unique_ptr<LLNode> node(new LLNode(1, mFonts, 3));
		node->setPosition(100.f + i * 180.f, 100.f);
		node->setVelocity(0.f, 0.f);
		if (i == 0) {
			node->setColorSquare(sf::Color::Red);
			node->setDirection(1);
		}
		if (i == mDllNodes.size() - 1) {
			node->setColorSquare(sf::Color::Green);
			node->setDirection(2);
		}
		node->setValue(mValue[i]);
		tmpDllNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
	}
	if (tmpDllNodes.size() == 1) mDllNodes[0]->setDirection(0);
	if (tmpStep <= operation.first) {
		tmpDllNodes[tmpStep-1]->setColor(sf::Color::Cyan);
		if (tmpStep == 1) mPseudocode->setColorText(1);
		else if (tmpStep == operation.first) {
			mPseudocode->setColorText(3);
			mPseudocode->setColorText(4);
			mPseudocode->setColorText(5);
		}
		else mPseudocode->setColorText(2);
	}
	else {
		mSceneLayers[Air]->detachChild(*tmpDllNodes[operation.first-1]);
		tmpDllNodes.erase(tmpDllNodes.begin() + operation.first - 1);
		for (int i = 0; i < tmpDllNodes.size(); ++i) {
			tmpDllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
			tmpDllNodes[i]->setColor(sf::Color::White);
		}
	}
}

void DllWorld::updateArrayStep() {
	if (step == 0) return;
	for (auto node : mDllNodes) {
		node->setColor(sf::Color::White);
	}
	mPseudocode->resetColor();
	if (step <= operation.first)
	{
		mDllNodes[step-1]->setColor(sf::Color::Cyan);
		if (step == 1) mPseudocode->setColorText(1);
		else mPseudocode->setColorText(2);
	}
	else
	{
		mDllNodes[operation.first-1]->setValue(operation.second);
		mPseudocode->setColorText(3);
	}
}

void DllWorld::searchArrayStep() {
	for (auto node : mDllNodes) 
		node->setColor(sf::Color::White);
	mPseudocode->resetColor();
	if (step == 0) return;
	int tmpStep = step;
	mPseudocode->setColorText(1);
	if (--tmpStep == 0) return;
	for (int i = 0; i < mDllNodes.size(); ++i) {
		if (mDllNodes[i]->getValue() == operation.second) {
			mDllNodes[i]->setColor(sf::Color::Green);
		}
		tmpStep--;
		if (tmpStep == 0) {
			mPseudocode->resetColor();
			mPseudocode->setColorText(2);
			mPseudocode->setColorText(3);
			if (mDllNodes[i]->getValue() == operation.second) mPseudocode->setColorText(4);
			mPseudocode->setColorText(5);
			mDllNodes[i]->setColor(sf::Color::Cyan);
			break;
		}
	}
	if (tmpStep == 0) return;
	if (mDllNodes[mDllNodes.size() - 1]->getValue() == operation.second) {
		mPseudocode->resetColor();
		mPseudocode->setColorText(3);
		mPseudocode->setColorText(4);
		mDllNodes[mDllNodes.size() - 1]->setColor(sf::Color::Green);
		tmpStep--;
	}
	if (tmpStep == 0) return;
	for (int i = 0; i < mDllNodes.size(); ++i) {
		mDllNodes[i]->setColor(sf::Color::White);
	}
}

void DllWorld::reUpdate() {
	isRunAtOnce = false;
	if (mPseudocode != nullptr) {
		mSceneLayers[Air]->detachChild(*mPseudocode);
		mPseudocode = nullptr;
	}
	if (operationType == 3 || operationType == 4) {
		operationType = 0;
		step = totalStep = 0;
		return;
	}
	operationType = 0;
	step = totalStep = 0;
	mDllNodes.clear();
	for (int i = 0; i < tmpDllNodes.size(); ++i) {
		if (i == 0) tmpDllNodes[i]->setColorSquare(sf::Color::Red);
		tmpDllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
		tmpDllNodes[i]->setColor(sf::Color::White);
		tmpDllNodes[i]->setDirection(3);
		tmpDllNodes[i]->setColorSquare(sf::Color::White);
		if (i == 0)
		{
			tmpDllNodes[i]->setColorSquare(sf::Color::Red);
			tmpDllNodes[i]->setDirection(1);
		}
		if (i == tmpDllNodes.size() - 1) {
			tmpDllNodes[i]->setColorSquare(sf::Color::Green);
			tmpDllNodes[i]->setDirection(2);
		}
		mDllNodes.push_back(tmpDllNodes[i]);
	}
	tmpDllNodes.clear();
	operation = { -1, -1 };
}

void DllWorld::next() {
	if (operationType == 0) return;

	step++;
	step = std::min(step, totalStep);

	if (operationType == 1) addToArrayStep();
	else if (operationType == 2) deleteFromArrayStep();
	else if (operationType == 3) updateArrayStep();
	else if (operationType == 4) searchArrayStep();

	if (step >= totalStep) reUpdate();
}

void DllWorld::previous() {
	if (operationType == 0) return;
	step--;
	step = std::max(step, 0);
	if (operationType == 1) addToArrayStep();
	else if (operationType == 2) deleteFromArrayStep();
	else if (operationType == 3) updateArrayStep();
	else if (operationType == 4) searchArrayStep();
}

void DllWorld::runAtOnce() {
	isRunAtOnce = true;
}

void DllWorld::loadFromFile() {
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