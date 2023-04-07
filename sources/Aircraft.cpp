#include <Aircraft.hpp>

Aircraft::Aircraft(int value, const FontHolder& fonts): mText()
{
	mText.setString(std::to_string(value));
	mText.setFont(fonts.get(Fonts::Main));
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mText, states);
}

unsigned int Aircraft::getCategory() const {
	return Category::PlayerAircraft;
}