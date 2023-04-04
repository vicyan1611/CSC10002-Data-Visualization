#include <Aircraft.hpp>

Textures::ID toTexture(Aircraft::Type type) {
	if (type == Aircraft::Eagle) {
		return Textures::Eagle;
	}
	else if (type == Aircraft::Raptor) {
		return Textures::Raptor;
	}
}

Aircraft::Aircraft(Type type, const TextureHolder& textures): mType(type), mSprite(textures.get(toTexture(type)))
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

unsigned int Aircraft::getCategory() const {
	switch (mType)
	{
	case Eagle:
		return Category::PlayerAircraft;
	default:
		return Category::EnemyAircraft;
	}
}