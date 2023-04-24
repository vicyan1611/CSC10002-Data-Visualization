#include <Pseudocode.hpp>

Pseudocode::Pseudocode(const FontHolder& fonts, int num) {
	mNum = num;

	mText1.setFont(fonts.get(Fonts::Main));
	mText1.setFillColor(sf::Color::White);
	mText1.setCharacterSize(18);

	mText2.setFont(fonts.get(Fonts::Main));
	mText2.setFillColor(sf::Color::White);
	mText2.setCharacterSize(18);

	mText3.setFont(fonts.get(Fonts::Main));
	mText3.setFillColor(sf::Color::White);
	mText3.setCharacterSize(18);

	mText4.setFont(fonts.get(Fonts::Main));
	mText4.setFillColor(sf::Color::White);
	mText4.setCharacterSize(18);

	mText5.setFont(fonts.get(Fonts::Main));
	mText5.setFillColor(sf::Color::White);
	mText5.setCharacterSize(18);

	setStringInside();
	setPositionInside();
}

void Pseudocode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mText1, states);
	target.draw(mText2, states);
	target.draw(mText3, states);
	if (mNum == 2 || mNum == 4 || mNum == 5) {
		target.draw(mText4, states);
		target.draw(mText5, states);
	}
}
void Pseudocode::setStringInside() {
	if (mNum == 1) {
		mText1.setString("for index: 1 -> n");
		mText2.setString("	if (arr[i] == value) then found[i] = true");
	}
	else if (mNum == 2) {
		mText1.setString("int* newArr = new int [sizeof(arr)+1]");
		mText2.setString("for index: 1 -> sizeof(newArr)");
		mText3.setString("	if (index < position) newArr[index] = arr[index];");
		mText4.setString("	if (index == position) newArr[index] = value;");
		mText5.setString("	if (index > position) newArr[index] = arr[index-1]");
	} 
	else if (mNum == 3) {
		mText1.setString("int* newArr = new int [sizeof(arr)-1]");
		mText2.setString("for index: 1 -> sizeof(newArr)");
		mText3.setString("	if (index < position) newArr[index] = arr[index];");
		mText4.setString("	if (index >= position) newArr[index] = arr[index+1]");
	}
	else if (mNum == 4) {
		mText1.setString("Node* cur = pHead, pre = nullptr");
		mText2.setString("for k: 0 -> position - 2 { pre = cur; cur = cur->pNext }");
		mText3.setString("newNode = createNode(value)");
		mText4.setString("newNode->pNext = cur");
		mText5.setString("pre->pNext = newNode");
	}
	else if (mNum == 5) {
		mText1.setString("Node* pre = pHead");
		mText2.setString("for k: 1 -> position - 2 { pre = pre->pNext }");
		mText3.setString("Node * del = pre->pNext");
		mText4.setString("pre->pNext = del->pNext");
		mText5.setString("delete del");
	}
	else if (mNum == 6) {
		mText1.setString("Node* cur = pHead");
		mText2.setString("for k: 0 -> position - 2 { cur = cur->pNext }");
		mText3.setString("cur->value = value");
	}
}

void Pseudocode::setPositionInside() {
	mText2.setPosition(mText1.getPosition().x, mText1.getPosition().y + 30.f);
	mText3.setPosition(mText1.getPosition().x, mText1.getPosition().y + 60.f);
	mText4.setPosition(mText1.getPosition().x, mText1.getPosition().y + 90.f);
	mText5.setPosition(mText1.getPosition().x, mText1.getPosition().y + 120.f);
}

void Pseudocode::setColorText(int num) {
	switch (num)
	{
		case 1:
			mText1.setFillColor(sf::Color::Cyan);
			break;
		case 2:
			mText2.setFillColor(sf::Color::Cyan);
			break;
		case 3:
			mText3.setFillColor(sf::Color::Cyan);
			break;
		case 4:
			mText4.setFillColor(sf::Color::Cyan);
			break;
		case 5:
			mText5.setFillColor(sf::Color::Cyan);
			break;
	}
}

void Pseudocode::resetColor() {
	mText1.setFillColor(sf::Color::White);
	mText2.setFillColor(sf::Color::White);
	mText3.setFillColor(sf::Color::White);
	mText4.setFillColor(sf::Color::White);
	mText5.setFillColor(sf::Color::White);
}