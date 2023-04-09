#include <LLNode.hpp>

LLNode::LLNode(int value, const FontHolder& fonts) : mText()
{
	mText.setString(std::to_string(value));
	mText.setFont(fonts.get(Fonts::Main));
	mText.setFillColor(sf::Color::White);
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	mSquare.setSize(sf::Vector2f(75.f, 75.f));
	mSquare.setFillColor(sf::Color::Transparent);
	mSquare.setOutlineColor(sf::Color::White);
	mSquare.setOutlineThickness(1.f);
	bounds = mSquare.getLocalBounds();
	mSquare.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void LLNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSquare, states);
	target.draw(mText, states);
}

unsigned int LLNode::getValue() const {
	std::string temp = mText.getString();
	return std::stoi(temp);
}

void LLNode::setColor(sf::Color color) {
	mText.setFillColor(color);
}

void LLNode::setValue(int value) {
	mText.setString(std::to_string(value));
}

void LLNode::setString(std::string str) {
	mText.setString(str);
}

sf::Color LLNode::getColor() const {
	return mText.getFillColor();
}