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
	/*for (auto player : mPlayerAircraftar) {
		mSceneLayers[Air]->detachChild(*player);
	}
	mPlayerAircraftar.clear();*/
	//delete mPlayerAircraftar;
	{
		int n = mPlayerAircraftar.getSize();
		for (int i = 1; i <= n; ++i) {
			mSceneLayers[Air]->detachChild(*mPlayerAircraftar.getNode(i)->data);
		}
		mPlayerAircraftar.reset();
	}

	for (int i = 0; i < data.size(); i++) {
		std::unique_ptr<Aircraft> player(new Aircraft(data[i], mFonts));
		player->setPosition(100.f + (i + 1) * 100.f, 100.f);
		player->setVelocity(0.f, 0.f);
		//mplayer..add(player.get());
		mPlayerAircraftar.addNode(player.get());
		mSceneLayers[Air]->attachChild(std::move(player));
	}
}

void StaticArrayWorld::setRandomArray() {
	//delete mPlayerAircraftar;
	{
		int n = mPlayerAircraftar.getSize();
		for (int i = 1; i <= n; ++i) {
			mSceneLayers[Air]->detachChild(*mPlayerAircraftar.getNode(i)->data);
		}
		mPlayerAircraftar.reset();
	}
	int size = rand() % 10 + 1;
	for (int i = 1; i <= size; i++) {
		int value = rand() % 100 + 1;
		std::unique_ptr<Aircraft> player(new Aircraft(value, mFonts));
		player->setPosition(100.f + i * 100.f, 100.f);
		player->setVelocity(0.f, 0.f);
		//mplayer..add(player.get());
		mPlayerAircraftar.addNode(player.get());
		mSceneLayers[Air]->attachChild(std::move(player));
	}
}

void StaticArrayWorld::updateArray(int id, int value) {
	//mplayerair..getsize();
	if (id < 1 || id > mPlayerAircraftar.getSize()) {
		return;
	}
	//get location then change value
	mPlayerAircraftar.getNode(id)->data->setValue(value);
}

void StaticArrayWorld::searchArray(int value) {
	totalSearchStep = int (mPlayerAircraftar.getSize()) + 1;
	//getlastoflinkedlist
	if (mPlayerAircraftar.getNode(mPlayerAircraftar.getSize())->data->getValue() == value) {
		totalSearchStep++;
	}
	searchValue = value;
	operationType = 1;
	step = 0;
	std::unique_ptr<Pseudocode> code(new Pseudocode(mFonts, 1));
	code->setPosition(100.f, 400.f);
	code->setVelocity(0.f, 0.f);
	mPseudocode = code.get();
	mSceneLayers[Air]->attachChild(std::move(code));
}

void StaticArrayWorld::runAtOnce() {
	if (operationType == 0) return;
	isRunAtOnce = true;
}

void StaticArrayWorld::searchArrayStep() {
	int n = mPlayerAircraftar.getSize();
	for (int i = 1; i <= n; ++i) {
		mPlayerAircraftar.getNode(i)->data->setColor(sf::Color::White);
	}
	mPseudocode->resetColor();
	if (step == 0) return;
	if (step <= mPlayerAircraftar.getSize()) {
		for (int i = 1; i <= step; ++i) {
			if (mPlayerAircraftar.getNode(i)->data->getValue() == searchValue) {
				mPlayerAircraftar.getNode(i)->data->setColor(sf::Color::Red);
			}
		}
		int tmpID = step;
		mPlayerAircraftar.getNode(tmpID)->data->setColor(sf::Color::Green);
		if (mPlayerAircraftar.getNode(tmpID)->data->getValue() == searchValue) mPseudocode->setColorText(2);
		else mPseudocode->setColorText(1);
		return;
	}
	if (step == mPlayerAircraftar.getSize() + 1 && totalSearchStep == mPlayerAircraftar.getSize() + 2) {
		for (int i = 1; i <= mPlayerAircraftar.getSize(); ++i) {
			if (mPlayerAircraftar.getNode(i)->data->getValue() == searchValue) {
				mPlayerAircraftar.getNode(i)->data->setColor(sf::Color::Red);
			}
		}
		mPseudocode->setColorText(2);
	}
}

void StaticArrayWorld::next() {
	if (operationType == 0) return;
	step++;
	step = std::min(step, totalSearchStep);

	searchArrayStep();
	if (step >= totalSearchStep) {
		operationType = 0;
		step = 0;
		totalSearchStep = 0;
		isRunAtOnce = 0;
		mSceneLayers[Air]->detachChild(*mPseudocode);
	}
}

void StaticArrayWorld::previous() {
	if (operationType == 0) return;
	step--;
	step = std::max(step, 0);
	searchArrayStep();
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