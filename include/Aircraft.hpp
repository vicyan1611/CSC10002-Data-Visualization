#pragma once
#include <Entity.hpp>
#include <ResourceHolder.hpp>

class Aircraft : public Entity {
public:
	enum Type
	{
		Eagle,
		Raptor,
	};
public:
	explicit Aircraft(int value, const FontHolder& fonts);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	unsigned int getCategory() const;
	unsigned int getValue() const;
	void setColor(sf::Color color);
	void setValue(int value);
	void setString(std::string str);
private:
	sf::Text mText;
	sf::RectangleShape mSquare;
};