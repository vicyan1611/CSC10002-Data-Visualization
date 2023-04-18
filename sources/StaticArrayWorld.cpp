#include <StaticArrayWorld.hpp>
#include <fstream>
#include <iostream>

StaticArrayWorld::StaticArrayWorld(sf::RenderWindow& window, int& id)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mCommandQueue()
	{
		loadTextures(id);
		buildScene();
	}
void StaticArrayWorld::loadTextures(int id) {
	if (id == 0) mTextures.load(Textures::Desert, "assets/TitleScreen.png");
	if (id == 1) mTextures.load(Textures::Desert, "assets/TitleScreen1.jpg");
	if (id == 2) mTextures.load(Textures::Desert, "assets/TitleScreen2.jpg");
	if (id == 3) mTextures.load(Textures::Desert, "assets/TitleScreen3.jpg");
	if (id == 4) mTextures.load(Textures::Desert, "assets/TitleScreen4.jpg");
	if (id == 5) mTextures.load(Textures::Desert, "assets/TitleScreen5.jpg");
	mTextures.load(Textures::Eagle, "assets/eagle.png");
	mFonts.load(Fonts::Main, "assets/JetBrainsMono-Medium.ttf");
}

void StaticArrayWorld::buildScene() {
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

	/*std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(100.f, 100.f);
	mPlayerAircraft->setVelocity(40.f, 0.f);
	mSceneLayers[Air]->attachChild(std::move(leader));*/
}

void StaticArrayWorld::loadFromFile() {
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

void StaticArrayWorld::setArray(std::vector<int> data) {
	for (auto player : mPlayerAircraftar) {
		mSceneLayers[Air]->detachChild(*player);
	}
	mPlayerAircraftar.clear();
	for (int i = 0; i < data.size(); i++) {
		std::unique_ptr<Aircraft> player(new Aircraft(data[i], mFonts));
		player->setPosition(100.f + (i + 1) * 100.f, 100.f);
		player->setVelocity(0.f, 0.f);
		mPlayerAircraftar.push_back(player.get());
		mSceneLayers[Air]->attachChild(std::move(player));
	}
}

void StaticArrayWorld::setRandomArray() {
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

void StaticArrayWorld::updateArray(int id, int value) {
	id--;
	if (id < 0 || id >= mPlayerAircraftar.size()) {
		return;
	}
	mPlayerAircraftar[id]->setValue(value);
}

void StaticArrayWorld::searchArray(int value) {
	totalSearchStep = 0;
	for (int i = 0; i < mPlayerAircraftar.size(); i++) {
		if (mPlayerAircraftar[i]->getValue() == value) {
			mPlayerAircraftar[i]->setColor(sf::Color::Cyan);
			break;
		}
		totalSearchStep++;
	}
	step = 0;
}

void StaticArrayWorld::runAtOnce() {
	isRunAtOnce = true;
}

void StaticArrayWorld::next() {
	if (step == totalSearchStep) {
		mPlayerAircraftar[step]->setColor(sf::Color::White);
		isRunAtOnce = false;
		return;
	}
	step++;
	step = std::min(step, totalSearchStep);
	int tmpID = step - 1;
	if (step > 0) mPlayerAircraftar[tmpID]->setColor(sf::Color::White);
	mPlayerAircraftar[step]->setColor(sf::Color::Red);	
}

void StaticArrayWorld::previous() {
	step--;
	step = std::max(step, 0);
	int tmpID = step + 1;
	if (step < totalSearchStep) mPlayerAircraftar[tmpID]->setColor(sf::Color::White);
	mPlayerAircraftar[step]->setColor(sf::Color::Red);
}

void StaticArrayWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& StaticArrayWorld::getCommandQueue() {
	return mCommandQueue;
}

void StaticArrayWorld::update(sf::Time dt, sf::Time& at) {
	while (!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	mSceneGraph.update(dt, at);
	if (at >= sf::seconds(1.f) && isRunAtOnce) {
		at = sf::Time::Zero;
		next();
	}
}