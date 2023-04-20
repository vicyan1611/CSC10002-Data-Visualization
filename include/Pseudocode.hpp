#pragma once
#include <Entity.hpp>
#include <ResourceHolder.hpp>

class Pseudocode : public Entity {
public:
	explicit Pseudocode(const FontHolder& fonts, int num);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void setLoopOutside();
	void setFound();
	void resetColor();
private:
	void setStringInside();
	void setOriginInside();
	void setPositionInside();
private:
	sf::Text mText1, mText2;
	int mNum = 0;
};