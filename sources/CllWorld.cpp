#include <CllWorld.hpp>
#include <iostream>
#include <fstream>

CllWorld::CllWorld(sf::RenderWindow& window, int& id)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
{
	loadTextures(id);
	buildScene();
}
 
void CllWorld::loadTextures(int id) {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	if (id == 0) mTextures.load(Textures::Desert, "assets/TitleScreen.png");
	if (id == 1) mTextures.load(Textures::Desert, "assets/TitleScreen1.jpg");
	if (id == 2) mTextures.load(Textures::Desert, "assets/TitleScreen2.jpg");
	if (id == 3) mTextures.load(Textures::Desert, "assets/TitleScreen3.jpg");
	if (id == 4) mTextures.load(Textures::Desert, "assets/TitleScreen4.jpg");
	if (id == 5) mTextures.load(Textures::Desert, "assets/TitleScreen5.jpg");
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
	/*std::unique_ptr<LLNode> startNode(new LLNode(0, mFonts, 0));
	startNode->setPosition(100.f, 100.f);
	startNode->setVelocity(0.f, 0.f);
	mSceneLayers[Air]->attachChild(std::move(startNode));*/

	//test Node
	/*std::unique_ptr<LLNode> CllNode(new LLNode(1, mFonts, 8));
	CllNode->setPosition(500.f, 100.f);
	CllNode->setCll(true);
	CllNode->setVelocity(0.f, 0.f);
	mSceneLayers[Air]->attachChild(std::move(CllNode));*/
}

void CllWorld::update(sf::Time dt, sf::Time& at) {
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	mSceneGraph.update(dt, at);
	if (at >= sf::seconds(1.f) && isRunAtOnce) {
		at = sf::Time::Zero;
		next();
	}
}

void CllWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& CllWorld::getCommandQueue() {
	return mCommandQueue;
}

void CllWorld::setArray(std::vector<int> data) {
	if (!mCllNodes.empty()) {
		for (auto& node : mCllNodes) {
			mSceneLayers[Air]->detachChild(*node);
		}
		mCllNodes.clear();
	}
	for (int i = 0; i < data.size(); ++i) {
		std::unique_ptr <LLNode> CllNode(new LLNode(data[i], mFonts, 1));
		CllNode->setPosition(100.f + i * 180.f, 100.f);
		if (i == 0) CllNode->setColorSquare(sf::Color::Red);
		if (i == data.size() - 1)
		{
			if (data.size() > 1) {
				CllNode->setDirection(8);
				CllNode->setCll(true);
			}
			else {
				CllNode->setDirection(0);
			}
		}
		mCllNodes .push_back(CllNode.get());
		mSceneLayers[Air]->attachChild(std::move(CllNode));
	}
}

void CllWorld::setRandomArray() {
	int n = rand() % 8 + 1;
	std::vector<int> data;
	for (int i = 0; i < n; ++i) {
		data.push_back(rand() % 100);
	}
	setArray(data);
}

void CllWorld::addToArray(int id, int value) {
	if (id < 1 || id > mCllNodes.size() + 1) return;
	if (mCllNodes.size() == 0) {
		std::vector <int> data;
		data.push_back(value);
		setArray(data);
		return;
	}
	operationType = 1;
	totalStep = id + 1;
	operation = { id, value };
	step = 0;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 4));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void CllWorld::addToArrayStep() {
	for (int i = 0; i < mCllNodes.size(); ++i)
		mCllNodes[i]->setColor(sf::Color::White);
	if (!tmpCllNodes.empty()) {
		for (auto node : tmpCllNodes)
			mSceneLayers[Air]->detachChild(*node);
		tmpCllNodes.clear();
	}
	mPseudocode->resetColor();
	if (step == 0) return;
	if (step < operation.first)
	{
		if (step == 1) mPseudocode->setColorText(1);
		else mPseudocode->setColorText(2);
		int tmpID = step - 1;
		mCllNodes[tmpID]->setColor(sf::Color::Cyan);
	} else if (step >= operation.first) {
		if (operation.first == mCllNodes.size() + 1) {
			std::unique_ptr<LLNode> node(new LLNode(operation.second, mFonts, 0));
			node->setPosition(mCllNodes[mCllNodes.size()-1]->getPosition() + sf::Vector2f(180.f, 0.f));
			node->setVelocity(0.f, 0.f);
			tmpCllNodes.push_back(node.get());
			mSceneLayers[Air]->attachChild(std::move(node));
		}
		else {
			int tmpID = operation.first - 1;
			mCllNodes[tmpID]->setColor(sf::Color::Cyan);
			std::unique_ptr<LLNode> node(new LLNode(operation.second, mFonts, 4));
			node->setPosition(mCllNodes[tmpID]->getPosition() + sf::Vector2f(0.f, 200.f));
			node->setVelocity(0.f, 0.f);
			tmpCllNodes.push_back(node.get());
			mSceneLayers[Air]->attachChild(std::move(node));
		}
		mPseudocode->setColorText(3);
		mPseudocode->setColorText(4);
	}
	if (step > operation.first) {
		mPseudocode->resetColor();
		mPseudocode->setColorText(5);
		int tmpID = operation.first - 1;
		tmpCllNodes[0]->setDirection(1);
		mCllNodes.insert(mCllNodes.begin() + tmpID, tmpCllNodes[0]);
		tmpCllNodes.clear();
		for (int i = 0; i < mCllNodes.size(); ++i) {
			mCllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
			mCllNodes[i]->setColor(sf::Color::White);
			mCllNodes[i]->setColorSquare(sf::Color::White);
			mCllNodes[i]->setCll(false);
			mCllNodes[i]->setDirection(1);
			if (i == mCllNodes.size() - 1)
			{
				mCllNodes[i]->setCll(true);
				mCllNodes[i]->setDirection(8);
			}
			if (i == 0) mCllNodes[i]->setColorSquare(sf::Color::Red); 
		}
	}
}

void CllWorld::deleteFromArray(int id) {
	if (id > mCllNodes.size() || id < 1) return;
	if (mCllNodes.size() == 1)
	{
		mSceneLayers[Air]->detachChild(*mCllNodes[0]);
		mCllNodes.clear();
		return;
	}
	operationType = 2;
	totalStep = id + 1;
	operation = { id, 0 };
	if (id == 1 || id == mCllNodes.size()) {
		totalStep++;
	}
	step = 0;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 5));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void CllWorld::deleteFromArrayStep() {
	for (int i = 0; i < mCllNodes.size(); ++i)
	{
		mCllNodes[i]->setColor(sf::Color::White);
		if (i != mCllNodes.size() - 1) mCllNodes[i]->setCll(false);
		if (i != 0) mCllNodes[i]->setColorSquare(sf::Color::White);
	}
	mPseudocode->resetColor();
	if (step == 0) return;
	if (step <= operation.first) {
		int tmpID = step - 1;
		mCllNodes[tmpID]->setColor(sf::Color::Cyan);
		if (step == 1) mPseudocode->setColorText(1);
		else if (step == operation.first)
		{
			mPseudocode->setColorText(3);
			mPseudocode->setColorText(4); mPseudocode->setColorText(5);
		}
		else mPseudocode->setColorText(2);
		return;
	}
	if (totalStep == operation.first + 1 && step == totalStep) {
		int tmpID = operation.first - 1;
		mSceneLayers[Air]->detachChild(*mCllNodes[tmpID]);
		mCllNodes.erase(mCllNodes.begin() + tmpID);
		for (int i = 0; i < mCllNodes.size(); ++i) {
			mCllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
			if (i == mCllNodes.size() - 1) {
				mCllNodes[i]->setCll(true);
			}
		}
		return;
	}
	if (operation.first == 1) {
		if (step >= 2) {
			mCllNodes[operation.first]->setColorSquare(sf::Color::Red);
		}
		if (step == 3) {
			mSceneLayers[Air]->detachChild(*mCllNodes[0]);
			mCllNodes.erase(mCllNodes.begin());
			for (int i = 0; i < mCllNodes.size(); ++i) {
				mCllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
				if (i == mCllNodes.size() - 1) {
					mCllNodes[i]->setCll(true);
				}
			}
		}
	}
	else if (operation.first == mCllNodes.size()) {
		if (step + 1 == totalStep) {
			int tmpID = operation.first - 2;
			mCllNodes[tmpID]->setDirection(8);
			mCllNodes[tmpID]->setCll(true);
		}
		else {
			int tmpID = operation.first - 1;
			mSceneLayers[Air]->detachChild(*mCllNodes[tmpID]);
			mCllNodes.pop_back();
			for (int i = 0; i < mCllNodes.size(); ++i) {
				mCllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
				if (i == mCllNodes.size() - 1) {
					mCllNodes[i]->setCll(true);
					mCllNodes[i]->setDirection(8);
				}
			}
		}
	}
}

void CllWorld::updateArray(int id, int value) {
	if (id > mCllNodes.size() || id < 1) return;
	operationType = 3;
	totalStep = id + 2;
	operation = { id, value };
	step = 0;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 6));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void CllWorld::updateArrayStep() {
	if (step == 0) return;
	for (auto node : mCllNodes) {
		node->setColor(sf::Color::White);
	}
	mPseudocode->resetColor();
	if (step <= operation.first) {
		int tmpID = step - 1;
		mCllNodes[tmpID]->setColor(sf::Color::Cyan);
		if (step == 1) mPseudocode->setColorText(1); 
		else mPseudocode->setColorText(2);
	}
	else {
		int tmpID = operation.first - 1;
		mCllNodes[tmpID]->setValue(operation.second);
		mPseudocode->setColorText(3);
	}
}

void CllWorld::searchArray(int value) {
	if (mCllNodes.empty()) return;
	operationType = 4;
	totalStep = mCllNodes.size() + 1;
	operation = { 0, value };
	step = 0;
	int tmpID = mCllNodes.size() - 1;
	if (mCllNodes[tmpID]->getValue() == value) totalStep++;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 7));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void CllWorld::searchArrayStep() {
	for (auto node : mCllNodes) {
		node->setColor(sf::Color::White);
	}
	mPseudocode->resetColor();
	if (step == 0) return;
	int tmpStep = step;
	mPseudocode->setColorText(1);
	if (--tmpStep == 0) return;
	for (int i = 0; i < mCllNodes.size(); ++i) {
		if (mCllNodes[i]->getValue() == operation.second) {
			mCllNodes[i]->setColor(sf::Color::Green);
		}
		tmpStep--;
		if (tmpStep == 0) {
			mPseudocode->resetColor();
			mCllNodes[i]->setColor(sf::Color::Cyan);
			mPseudocode->setColorText(2);
			mPseudocode->setColorText(3);
			mPseudocode->setColorText(5);
			if (mCllNodes[i]->getValue() == operation.second) mPseudocode->setColorText(4);
			break;
		}
	}
	if (tmpStep == 0) return;
	if (mCllNodes[mCllNodes.size() - 1]->getValue() == operation.second) {
		mCllNodes[mCllNodes.size() - 1]->setColor(sf::Color::Green);
		mPseudocode->resetColor();
		mPseudocode->setColorText(3);
		mPseudocode->setColorText(4);
		tmpStep--;
	}
	if (tmpStep == 0) return;
	for (int i = 0; i < mCllNodes.size(); ++i) {
		mCllNodes[i]->setColor(sf::Color::White);
	}
}

void CllWorld::reUpdate() {
	operationType = 0;
	step = totalStep = 0;
	isRunAtOnce = false;
	if (mPseudocode != nullptr) {
		mSceneLayers[Air]->detachChild(*mPseudocode);
		mPseudocode = nullptr;
	}
	if (!tmpCllNodes.empty()) {
		for (auto& node : tmpCllNodes)
		{
			mSceneLayers[Air]->detachChild(*node);
		}
		tmpCllNodes.clear();
	}
}

void CllWorld::next() {
	if (operationType == 0) return;

	step++;
	step = std::min(step, totalStep);

	if (operationType == 1) addToArrayStep();
	else if (operationType == 2) deleteFromArrayStep();
	else if (operationType == 3) updateArrayStep();
	else if (operationType == 4) searchArrayStep();

	if (step >= totalStep) reUpdate();
}

void CllWorld::previous() {
	if (operationType == 0) return;
	
	step--;
	step = std::max(step, 0);

	if (operationType == 1) addToArrayStep();
	else if (operationType == 2) deleteFromArrayStep();
	else if (operationType == 3) updateArrayStep();
	else if (operationType == 4) searchArrayStep();
}

void CllWorld::runAtOnce() {
	isRunAtOnce = true;
}

void CllWorld::loadFromFile() {
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