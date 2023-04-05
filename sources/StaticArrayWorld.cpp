#include <StaticArrayWorld.hpp>

StaticArrayWorld::StaticArrayWorld(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mPlayerAircraft()
	, mCommandQueue()
	{
		loadTextures();
		buildScene();
	}
void StaticArrayWorld::loadTextures() {
	mTextures.load(Textures::Desert, "assets/TitleScreen.png");
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
}

void StaticArrayWorld::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& StaticArrayWorld::getCommandQueue() {
	return mCommandQueue;
}

void StaticArrayWorld::update(sf::Time dt) {
	while (!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	mSceneGraph.update(dt);
}