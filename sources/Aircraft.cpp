#include <Aircraft.hpp>

Aircraft::Aircraft(int value, const FontHolder& fonts): mText()
{
	mText.setString(std::to_string(value));
	mText.setFont(fonts.get(Fonts::Main));
	mText.setFillColor(sf::Color::White);
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mText, states);
}

unsigned int Aircraft::getCategory() const {
	return Category::PlayerAircraft;
}

unsigned int Aircraft::getValue() const {
	std::string temp = mText.getString();
	return std::stoi(temp);
}

void Aircraft::setColor(sf::Color color) {
	mText.setFillColor(color);
}

void Aircraft::setValue(int value) {
	mText.setString(std::to_string(value));
}