#include <LLNode.hpp>
#include <iostream>

LLNode::LLNode(int value, const FontHolder& fonts, int num) : mText()
{
	mText.setString(std::to_string(value));
	mText.setFont(fonts.get(Fonts::Main));
	mText.setCharacterSize(19);
	mText.setFillColor(sf::Color::White);
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	mSquare.setSize(sf::Vector2f(75.f, 75.f));
	mSquare.setFillColor(sf::Color::Transparent);
	mSquare.setOutlineColor(sf::Color::White);
	mSquare.setOutlineThickness(1.f);
	bounds = mSquare.getLocalBounds();
	mSquare.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	setDirection(num);
	setArrows();
}

void LLNode::setDirection(int num) {
	if (num & 1) mHasRight = true; else mHasRight = false;
	if ((num >> 1) & 1) mHasLeft = true; else mHasLeft = false;
	if ((num >> 2) & 1) mHasUp = true; else mHasUp = false;
	if ((num >> 3) & 1) mHasDown = true; else mHasDown = false;
}

void LLNode::setArrows() {
	mArrowRight.setSize(sf::Vector2f(90, 2));
	mArrowRight.setFillColor(sf::Color::Cyan); 
	sf::FloatRect bounds = mArrowRight.getLocalBounds();
	mArrowRight.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mArrowRight.setPosition(mSquare.getPosition().x + float(82.5), mSquare.getPosition().y - 10);
	
	mArrowLeft.setSize(sf::Vector2f(90, 2));
	mArrowLeft.setFillColor(sf::Color::Magenta);
	bounds = mArrowLeft.getLocalBounds();
	mArrowLeft.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mArrowLeft.setPosition(mSquare.getPosition().x - float(82.5), mSquare.getPosition().y + 10);

	mArrowUp.setSize(sf::Vector2f(2, 45));
	mArrowUp.setFillColor(sf::Color::Yellow);
	bounds = mArrowUp.getLocalBounds();
	mArrowUp.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mArrowUp.setPosition(mSquare.getPosition().x, mSquare.getPosition().y - 62.f);

	mArrowDown.setSize(sf::Vector2f(2, 45));
	mArrowDown.setFillColor(sf::Color::Red);
	bounds = mArrowDown.getLocalBounds();
	mArrowDown.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mArrowDown.setPosition(mSquare.getPosition().x, mSquare.getPosition().y + 62.f);

	//std::cout << mSquare.getPosition().x << " " << mSquare.getPosition().y << std::endl;
}

void LLNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	if (mText.getString() != "nullptr") target.draw(mSquare, states);
	target.draw(mText, states);
	if (mHasRight) target.draw(mArrowRight, states);
	if (mHasLeft) target.draw(mArrowLeft, states);
	if (mHasUp) target.draw(mArrowUp, states);
	if (mHasDown) target.draw(mArrowDown, states);
	if (isCll) {
		target.draw(mCll1, states);
		target.draw(mCll2, states);
	}
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
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void LLNode::setString(std::string str) {
	mText.setString(str);
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mText.setPosition(mSquare.getPosition().x, mSquare.getPosition().y);
}

sf::Color LLNode::getColor() const {
	return mText.getFillColor();
}

void LLNode::setColorSquare(sf::Color color) {
	mSquare.setOutlineColor(color);
}

void LLNode::setCll(bool cll) {
	isCll = cll;
	float length = getPosition().x - 100.f;
	//std::cout << length << std::endl;

	mCll1.setSize(sf::Vector2f(length, 2));
	mCll1.setFillColor(sf::Color::Red);
	sf::FloatRect bounds = mCll1.getLocalBounds();
	mCll1.setOrigin(bounds.width + getPosition().x, bounds.height / 2.f);
	mCll1.setPosition(getPosition().x, 83.f);

	mCll2.setSize(sf::Vector2f(2, 35.f));
	mCll2.setFillColor(sf::Color::Red);
	bounds = mCll2.getLocalBounds();
	mCll2.setOrigin(bounds.width + getPosition().x, bounds.height / 2.f);
	mCll2.setPosition(100.f, 64.f);
}