#include <TitleState.hpp>

TitleState::TitleState(StateStack& stack, Context context)
	:State(stack, context)
	, mText()
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	mText.setFont(context.fonts->get(Fonts::Main));
	mText.setString("Press Space to start");

	author.setFont(context.fonts->get(Fonts::Main));
	author.setString("Author: Pham Thanh Vinh - 22127459 - 22CLC02");

	title.setFont(context.fonts->get(Fonts::Main));
	title.setString("Data Visualization");

	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(std::floor(bounds.left + bounds.width/2.f), std::floor(bounds.top + bounds.height / 2.f));
	mText.setPosition(context.window->getView().getSize() / 2.f);

	title.setPosition(100.f, 100.f);
	author.setPosition(100.f, 200.f);
}

void TitleState::draw() {
	sf::RenderWindow &window = *getContext().window;
	window.draw(mBackgroundSprite);
	window.draw(author);
	window.draw(title);
	if (mShowText)
		window.draw(mText);
}

bool TitleState::update(sf::Time dt, sf::Time& at) {
	mTextEffectTime += dt;
	if (mTextEffectTime >= sf::seconds(0.5f)) {
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}
	return true;
}

bool TitleState::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		requestStackPop();
		requestStackPush(States::Menu);
	}
	return true;
}