#include <DynamicArrayWorld.hpp>
#include <iostream>
#include <fstream>

DynamicArrayWorld::DynamicArrayWorld(sf::RenderWindow& window, int& id)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
	{
		loadTextures(id);
		buildScene();
	}
void DynamicArrayWorld::loadTextures(int id) {
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
	if (id == 0) mTextures.load(Textures::Desert, "assets/TitleScreen.png");
	if (id == 1) mTextures.load(Textures::Desert, "assets/TitleScreen1.jpg");
	if (id == 2) mTextures.load(Textures::Desert, "assets/TitleScreen2.jpg");
	if (id == 3) mTextures.load(Textures::Desert, "assets/TitleScreen3.jpg");
	if (id == 4) mTextures.load(Textures::Desert, "assets/TitleScreen4.jpg");
	if (id == 5) mTextures.load(Textures::Desert, "assets/TitleScreen5.jpg");
}

void DynamicArrayWorld::loadFromFile() {
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
	if (id < 1|| id > mPlayerAircraftar.size() + 1) {
		return;
	}
	operationType = 1;
	totalSearchStep = mPlayerAircraftar.size() + 3;
	step = 0;
	operation = {id-1, value};
	std::cout << id << " " << value << std::endl;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 2));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void DynamicArrayWorld::deleteFromArray(int id) {
	if (id < 1 || id > mPlayerAircraftar.size()) {
		return;
	}
	operationType = 2;
	totalSearchStep = 1 + (int(mPlayerAircraftar.size()) - 1) + 1;
	step = 0;
	operation = { id - 1, 0 };
	std::cout << id << std::endl;

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 3));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void DynamicArrayWorld::addToArrayStep() {
	if (!tmp_mPlayerAircraftar.empty()) {
		for (auto player : tmp_mPlayerAircraftar) {
			mSceneLayers[Air]->detachChild(*player);
		}
		tmp_mPlayerAircraftar.clear();
	}
	mPseudocode->resetColor();
	if (step == 0) return;
	int tmp_step = step;
	for (int i = 0; i < mPlayerAircraftar.size() + 1; ++i) {
		std::unique_ptr<Aircraft> player(new Aircraft(1, mFonts));
		player->setPosition(100.f + (i + 1) * 100.f, 200.f);
		player->setVelocity(0.f, 0.f);
		player->setString("");
		tmp_mPlayerAircraftar.push_back(player.get());
		mSceneLayers[Air]->attachChild(std::move(player));
	}
	mPseudocode->setColorText(1);
	tmp_step--;
	if (tmp_step == 0) return; else mPseudocode->resetColor();
	mPseudocode->setColorText(2);
	for (int i = 0; i < tmp_mPlayerAircraftar.size(); ++i) {
		tmp_step--;
		if (i < operation.first) {
			tmp_mPlayerAircraftar[i]->setValue(mPlayerAircraftar[i]->getValue());
			if (tmp_step == 0) mPseudocode->setColorText(3);
		}
		else if (i == operation.first) {
			tmp_mPlayerAircraftar[i]->setValue(operation.second);
			if (tmp_step == 0) mPseudocode->setColorText(4);
		}
		else {
			int tmpID = i - 1;
			tmp_mPlayerAircraftar[i]->setValue(mPlayerAircraftar[tmpID]->getValue());
			if (tmp_step == 0) mPseudocode->setColorText(5);
		}
		if (tmp_step == 0) return;
	}
}

void DynamicArrayWorld::reUpdate() {
	isRunAtOnce = false;
	if (mPseudocode != nullptr) {
		mSceneLayers[Air]->detachChild(*mPseudocode);
		mPseudocode = nullptr;
	}
		if (operationType == 3) {
		operationType = 0;
		step = 0;
		totalSearchStep = 0;
		for (auto player : mPlayerAircraftar) {
			player->setColor(sf::Color::White);
		}
	}
	else {
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

void DynamicArrayWorld::deleteFromArrayStep() {
	if (!tmp_mPlayerAircraftar.empty()) {
		for (auto player : tmp_mPlayerAircraftar) {
			mSceneLayers[Air]->detachChild(*player);
		}
		tmp_mPlayerAircraftar.clear();
	}
	mPseudocode->resetColor();
	if (step == 0) return;
	int tmp_step = step;
	for (int i = 0; i < mPlayerAircraftar.size()-1; ++i) {
		std::unique_ptr<Aircraft> player(new Aircraft(mPlayerAircraftar[i]->getValue(), mFonts));
		player->setString("");
		player->setPosition(100.f + (i + 1) * 100.f, 200.f);
		player->setVelocity(0.f, 0.f);
		tmp_mPlayerAircraftar.push_back(player.get());
		mSceneLayers[Air]->attachChild(std::move(player));
	}
	mPseudocode->setColorText(1);
	tmp_step--;
	if (tmp_step == 0) return; else mPseudocode->resetColor();
	mPseudocode->setColorText(2);
	for (int i = 0; i < mPlayerAircraftar.size(); ++i) {
		if (i == operation.first) continue;
		tmp_step--;
		if (i < operation.first) {
			tmp_mPlayerAircraftar[i]->setPosition(100.f + (i+1) * 100.f, 200.f);
			tmp_mPlayerAircraftar[i]->setValue(mPlayerAircraftar[i]->getValue());
			if (tmp_step == 0) mPseudocode->setColorText(3);
		}
		else {
			int tmpID = i - 1;
			tmp_mPlayerAircraftar[tmpID]->setPosition(100.f + i * 100.f, 200.f);
			tmp_mPlayerAircraftar[tmpID]->setValue(mPlayerAircraftar[i]->getValue());
			if (tmp_step == 0) mPseudocode->setColorText(4);
		}
		if (tmp_step == 0) return;
	}
}

void DynamicArrayWorld::updateArray(int id, int value) {
	id--;
	if (id < 0 || id >= mPlayerAircraftar.size()) return;
	mPlayerAircraftar[id]->setValue(value);
}

void DynamicArrayWorld::searchArray(int value) {
	operationType = 3;
	step = 0;
	totalSearchStep = int(mPlayerAircraftar.size()) + 1;
	if (mPlayerAircraftar[mPlayerAircraftar.size() - 1]->getValue() == value) totalSearchStep++;

	operation = { -1, value };

	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 1));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void DynamicArrayWorld::searchArrayStep()
{
	for (auto player : mPlayerAircraftar) {
		player->setColor(sf::Color::White);
	}
	mPseudocode->resetColor();
	if (step == 0) return;
	if (step <= mPlayerAircraftar.size()) {
		for (int i = 0; i < step; ++i) {
			if (mPlayerAircraftar[i]->getValue() == operation.second) {
				mPlayerAircraftar[i]->setColor(sf::Color::Red);
			}
		}
		int tmpID = step - 1;
		mPlayerAircraftar[tmpID]->setColor(sf::Color::Green);
		if (mPlayerAircraftar[tmpID]->getValue() == operation.second) mPseudocode->setColorText(2);
		else mPseudocode->setColorText(1);
		return;
	}
	if (step == mPlayerAircraftar.size() + 1 && totalSearchStep == mPlayerAircraftar.size() + 2) {
		for (int i = 0; i < mPlayerAircraftar.size(); ++i) {
			if (mPlayerAircraftar[i]->getValue() == operation.second) {
				mPlayerAircraftar[i]->setColor(sf::Color::Red);
			}
		}
		mPseudocode->setColorText(2);
	}
}

void DynamicArrayWorld::next() {
	if (operationType == 0) return;

	step++;
	step = std::min(step, totalSearchStep);

	if (operationType == 1) addToArrayStep();
	else if (operationType == 2) deleteFromArrayStep();
	else if (operationType == 3) searchArrayStep();

	if (step >= totalSearchStep)  reUpdate();
}

void DynamicArrayWorld::previous() {
	if (operationType == 0) return;
	step--;
	step = std::max(step, 0);
	if (operationType == 1) addToArrayStep();
	else if (operationType == 2) deleteFromArrayStep();
	else if (operationType == 3) searchArrayStep();
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

void DynamicArrayWorld::update(sf::Time dt, sf::Time& at) {
	while (!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	mSceneGraph.update(dt, at);
	if (at >= sf::seconds(1.f) && isRunAtOnce) {
		at = sf::Time::Zero;
		next();
	}
}

void DynamicArrayWorld::runAtOnce() {
	isRunAtOnce = true;
}

void DynamicArrayWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& DynamicArrayWorld::getCommandQueue() {
	return mCommandQueue;
}