#include <Pseudocode.hpp>

Pseudocode::Pseudocode(const FontHolder& fonts, int num) {
	mNum = num;

	mText1.setFont(fonts.get(Fonts::Main));
	mText1.setFillColor(sf::Color::White);
	mText1.setCharacterSize(18);

	mText2.setFont(fonts.get(Fonts::Main));
	mText2.setFillColor(sf::Color::White);
	mText2.setCharacterSize(18);

	setStringInside();
	setPositionInside();
}

void Pseudocode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	if (mNum == 1) {
		target.draw(mText1, states);
		target.draw(mText2, states);
	}
}
void Pseudocode::setStringInside() {
	if (mNum == 1) {
		mText1.setString("for index: 1 -> n");
		mText2.setString("	if (arr[i] == value) then found[i] = true");
	}
}

void Pseudocode::setPositionInside() {
	mText2.setPosition(mText1.getPosition().x, mText1.getPosition().y + 30.f);
}

void Pseudocode::setOriginInside() {
	sf::FloatRect bounds = mText1.getLocalBounds();
	mText1.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	bounds = mText2.getLocalBounds();
	mText2.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Pseudocode::setLoopOutside() {
	mText1.setFillColor(sf::Color::Cyan);
	mText2.setFillColor(sf::Color::White);
}

void Pseudocode::setFound() {
	mText1.setFillColor(sf::Color::White);
	mText2.setFillColor(sf::Color::Cyan);
}

void Pseudocode::resetColor() {
	mText1.setFillColor(sf::Color::White);
	mText2.setFillColor(sf::Color::White);
}