#include <InputBox.hpp>
#include <iostream>

GUI::InputBox::InputBox(const FontHolder& fonts)
	: mBox(sf::Vector2f(200.f, 30.f))
	, mText("", fonts.get(Fonts::Main), 16)
	, mIsSelected(false)
{
	mBox.setFillColor(sf::Color(255, 255, 255, 100));
	mBox.setOutlineColor(sf::Color::Black);
	mBox.setOutlineThickness(1.f);
	sf::FloatRect bounds = mBox.getLocalBounds();
	mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

bool GUI::InputBox::isSelectable() const {
	return true;
}

void GUI::InputBox::select() {
	Component::select();
	mIsSelected = true;
	mBox.setOutlineColor(sf::Color::White);
}

void GUI::InputBox::deselect() {
	Component::deselect();
	mIsSelected = false;
	mBox.setOutlineColor(sf::Color::Black);
}

void GUI::InputBox::activate() {
	Component::activate();
	mBox.setOutlineColor(sf::Color::Red);
}

void GUI::InputBox::deactivate() {
	Component::deactivate();
	mBox.setOutlineColor(sf::Color::Black);
}

void GUI::InputBox::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::TextEntered && mIsSelected) {
		if (event.text.unicode == '\b' && mText.getString().getSize() > 0) {
			mText.setString(mText.getString().substring(0, mText.getString().getSize() - 1));
			//centerOrigin
			sf::FloatRect bounds = mText.getLocalBounds();
			mText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
		}
		else {
			if (event.text.unicode == 9) {
				deactivate();
				mFinalText.setString(mText.getString());
				mText.setString("");
				select();
			}
			else {
				mText.setString(mText.getString() + static_cast<char>(event.text.unicode));
				//centerOrigin
				sf::FloatRect bounds = mText.getLocalBounds();
				mText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
			}
		}
	}
}

void GUI::InputBox::setText(const std::string& text) {
	mText.setString(text);
	//centerOrigin
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

std::string GUI::InputBox::getText() const {
	return mText.getString();
}

std::string GUI::InputBox::getFinalText() {
	std::string temp = mFinalText.getString();
	mFinalText.setString("");
	return temp;
}

void GUI::InputBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mBox, states);
	target.draw(mText, states);
}