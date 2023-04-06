#pragma once
#include <Component.hpp>
#include <ResourceHolder.hpp>

namespace GUI {
	class InputBox : public Component {
	public:
		typedef std::shared_ptr<InputBox> Ptr;
	public:
		InputBox(const FontHolder& fonts);
		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();

		virtual void activate();
		virtual void deactivate();

		virtual void handleEvent(const sf::Event& event);

		void setText(const std::string& text);
		std::string getText() const;
		std::string getFinalText();
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:
		sf::RectangleShape mBox;
		sf::Text mText;
		bool mIsSelected;
		sf::Text mFinalText;
	};
}
