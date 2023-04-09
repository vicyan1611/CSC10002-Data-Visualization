#pragma once
#include <Entity.hpp>
#include <ResourceHolder.hpp>

class LLNode : public Entity {
public:
	explicit LLNode(int value, const FontHolder& fonts);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	unsigned int getValue() const;
	void setColor(sf::Color color);
	sf::Color getColor() const;
	void setValue(int value);
	void setString(std::string str);
private:
	sf::Text mText;
	sf::RectangleShape mSquare, mArrowLeft, mArrowRight;
};