#include <DynamicArrayWorld.hpp>
#include <iostream>

DynamicArrayWorld::DynamicArrayWorld(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
	{
		loadTextures();
		buildScene();
	}
void DynamicArrayWorld::loadTextures() {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	mTextures.load(Textures::Desert, "assets/GameState.jpg");
}

void DynamicArrayWorld::setRandomArray() {
	for (auto player : mPlayerAircraftar) {
		mSceneLayers[Air]->detachChild(*player);
	}
	mPlayerAircraftar.clear();
	int size = rand() % 10 + 1;
	for (int i = 1; i <= size; i++) {
		int value = rand() % 100 + 1;
		std::unique_ptr<Aircraft> player(new Aircraft(value, mFonts));
		player->setPosition(100.f + i * 100.f, 100.f);
		player->setVelocity(0.f, 0.f);
		mPlayerAircraftar.push_back(player.get());
		mSceneLayers[Air]->attachChild(std::move(player));
	}
}

void DynamicArrayWorld::setArray(std::vector<int> data) {
	for (auto player : mPlayerAircraftar) {
		mSceneLayers[Air]->detachChild(*player);
	}
	mPlayerAircraftar.clear();
	for (int i = 0; i < data.size(); ++i) {
		std::unique_ptr<Aircraft> player(new Aircraft(data[i], mFonts));
		player->setPosition(100.f + (i + 1) * 100.f, 100.f);
		player->setVelocity(0.f, 0.f);
		mPlayerAircraftar.push_back(player.get());
		mSceneLayers[Air]->attachChild(std::move(player));
	}
}

void DynamicArrayWorld::addToArray(int id, int value) {
	if (id < 1|| id > mPlayerAircraftar.size()) {
		return;
	}
	operationType = 1;
	totalSearchStep = 1 + (mPlayerAircraftar.size() - id + 1) + 1 + 1;
	step = 0;
	operation = {id-1, value};
	std::cout << id << " " << value << std::endl;
}

void DynamicArrayWorld::deleteFromArray(int id) {
	if (id < 1 || id > mPlayerAircraftar.size()) {
		return;
	}
	operationType = 2;
}

void DynamicArrayWorld::addToArrayStep() {
	if (!tmp_mPlayerAircraftar.empty()) {
		for (auto player : tmp_mPlayerAircraftar) {
			mSceneLayers[Air]->detachChild(*player);
		}
		tmp_mPlayerAircraftar.clear();
	}
	int tmp_step = step;
	for (int i = 0; i < mPlayerAircraftar.size(); ++i) {
		std::unique_ptr<Aircraft> player(new Aircraft(mPlayerAircraftar[i]->getValue(), mFonts));
		player->setPosition(100.f + (i + 1) * 100.f, 200.f);
		player->setVelocity(0.f, 0.f);
		tmp_mPlayerAircraftar.push_back(player.get());
		mSceneLayers[Air]->attachChild(std::move(player));
	}
	{
		std::unique_ptr<Aircraft> player(new Aircraft(-1, mFonts));
		player->setPosition(100.f + (mPlayerAircraftar.size() + 1) * 100.f, 200.f);
		player->setVelocity(0.f, 0.f);
		tmp_mPlayerAircraftar.push_back(player.get());
		mSceneLayers[Air]->attachChild(std::move(player));
	}

	tmp_step--;
	if (tmp_step == 0) return;
	int tmpID = tmp_mPlayerAircraftar.size() - 1;
	mSceneLayers[Air]->detachChild(*tmp_mPlayerAircraftar[tmpID]);
	tmp_mPlayerAircraftar.pop_back();
	for (int i = tmp_mPlayerAircraftar.size() - 1; i >= operation.first; --i) {
		tmp_mPlayerAircraftar[i]->setPosition(100.f + (i + 2) * 100.f, 200.f);
		tmp_step--;
		if (tmp_step == 0) return;
	}
	std::unique_ptr<Aircraft> player(new Aircraft(operation.second, mFonts));
	player->setPosition(100.f + (operation.first + 1) * 100.f, 200.f);
	player->setVelocity(0.f, 0.f);
	tmp_mPlayerAircraftar.insert(tmp_mPlayerAircraftar.begin() + operation.first, player.get());
	mSceneLayers[Air]->attachChild(std::move(player));
	tmp_step--;
	if (tmp_step == 0) return;
}

void DynamicArrayWorld::next() {
	if (operationType == 1) {
		step++;
		step = std::min(step, totalSearchStep);
		addToArrayStep();
		if (step >= totalSearchStep) {
			operationType = 0;
			step = 0;
			totalSearchStep = 0;
			for (auto player : mPlayerAircraftar) {
				mSceneLayers[Air]->detachChild(*player);
			}
			mPlayerAircraftar.clear();
			for (int i = 0; i < tmp_mPlayerAircraftar.size(); ++i) {
				mPlayerAircraftar.push_back(tmp_mPlayerAircraftar[i]);
				mPlayerAircraftar[i]->setPosition(100.f + (i + 1) * 100.f, 100.f);
			}
			tmp_mPlayerAircraftar.clear();
			operation = { -1, -1 };
		}
	}
}

void DynamicArrayWorld::previous() {
	if (operationType == 1) {
		step--;
		step = std::max(step, 0);
		if (step != 0) addToArrayStep();
	}
}

void DynamicArrayWorld::buildScene() {
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

void DynamicArrayWorld::update(sf::Time dt) {
	while (!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	mSceneGraph.update(dt);
}

void DynamicArrayWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& DynamicArrayWorld::getCommandQueue() {
	return mCommandQueue;
}