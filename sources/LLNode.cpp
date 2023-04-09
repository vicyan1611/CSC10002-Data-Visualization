#include <LLNode.hpp>

LLNode::LLNode(int value, const FontHolder& fonts, int num) : mText()
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

	mHasRight = true;
	if (num == 2) mHasLeft = true;
	else mHasLeft = false;
	
	setArrows();
}

void LLNode::setArrows() {
	mArrowRight.setSize(sf::Vector2f(90, 2));
	mArrowRight.setFillColor(sf::Color::Cyan); 
	sf::FloatRect bounds = mArrowRight.getLocalBounds();
	mArrowRight.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mArrowRight.setPosition(mSquare.getPosition().x + 82.5, mSquare.getPosition().y - 10);
	
	mArrowLeft.setSize(sf::Vector2f(90, 2));
	mArrowLeft.setFillColor(sf::Color::Magenta);
	bounds = mArrowLeft.getLocalBounds();
	mArrowLeft.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mArrowLeft.setPosition(mSquare.getPosition().x - 82.5, mSquare.getPosition().y + 10);
}

void LLNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSquare, states);
	target.draw(mText, states);
	if (mHasRight) target.draw(mArrowRight, states);
	if (mHasLeft) target.draw(mArrowLeft, states);
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