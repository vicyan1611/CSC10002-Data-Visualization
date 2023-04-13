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
			CllNode->setColorSquare(sf::Color::Green);
			if (data.size() > 1) {
				CllNode->setDirection(8);
				CllNode->setCll(true);
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
	if (id < 1 || id > mCllNodes.size()) return;
	operationType = 1;
	totalStep = id + 1;
	operation = { id, value };
	step = 0;
}

void CllWorld::addToArrayStep() {
	for (int i = 0; i < mCllNodes.size(); ++i)
		mCllNodes[i]->setColor(sf::Color::White);
	if (!tmpCllNodes.empty()) {
		for (auto node : tmpCllNodes)
			mSceneLayers[Air]->detachChild(*node);
		tmpCllNodes.clear();
	}
	if (step == 0) return;
	if (step < operation.first)
	{
		int tmpID = step - 1;
		mCllNodes[tmpID]->setColor(sf::Color::Cyan);
	} else if (step >= operation.first) {
		int tmpID = operation.first - 1;
		mCllNodes[tmpID]->setColor(sf::Color::Cyan);
		std::unique_ptr<LLNode> node(new LLNode(operation.second, mFonts, 1));
		node->setPosition(mCllNodes[tmpID]->getPosition() + sf::Vector2f(0.f, 200.f));
		node->setVelocity(0.f, 0.f);
		tmpCllNodes.push_back(node.get());
		mSceneLayers[Air]->attachChild(std::move(node));
	}
	if (step > operation.first) {
		int tmpID = operation.first - 1;
		mCllNodes.insert(mCllNodes.begin() + tmpID, tmpCllNodes[0]);
		tmpCllNodes.clear();
		for (int i = 0; i < mCllNodes.size(); ++i) {
			mCllNodes[i]->setPosition(100.f + i * 180.f, 100.f);
			mCllNodes[i]->setColor(sf::Color::White);
			mCllNodes[i]->setColorSquare(sf::Color::White);
			if (i == mCllNodes.size() - 1)
			{
				mCllNodes[i]->setCll(true);
				mCllNodes[i]->setColorSquare(sf::Color::Green);
			}
			if (i == 0) mCllNodes[i]->setColorSquare(sf::Color::Red); 
		}
	}
}

void CllWorld::reUpdate() {
	operationType = 0;
	step = totalStep = 0;
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

	if (step >= totalStep) reUpdate();
}

void CllWorld::previous() {
	if (operationType == 0) return;
	
	step--;
	step = std::max(step, 0);

	if (operationType == 1) addToArrayStep();
}