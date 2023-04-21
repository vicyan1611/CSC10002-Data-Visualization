#pragma once
#include <Entity.hpp>
#include <ResourceHolder.hpp>

class Pseudocode : public Entity {
public:
	explicit Pseudocode(const FontHolder& fonts, int num);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void setColorText(int num);
	void resetColor();
private:
	void setStringInside();
	void setPositionInside();
private:
	sf::Text mText1, mText2, mText3, mText4, mText5;
	int mNum = 0;
};