#include <DynamicArrayWorld.hpp>

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