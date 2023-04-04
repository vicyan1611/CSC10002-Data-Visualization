#pragma once
#include <SFML/Graphics.hpp>
#include <CommandQueue.hpp>
#include <Aircraft.hpp>
#include <iostream>
#include <map>
class Player {
public:
	Player();
	enum Action {
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		ActionCount
	};
public:
	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);
	
	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;
private:
	static bool isRealtimeAction(Action action);
	std::map<sf::Keyboard::Key, Action> mKeyBinding;
	std::map<Action, Command> mActionBinding;
};