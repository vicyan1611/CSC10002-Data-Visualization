#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include <cassert>
#include <SFML/System/Time.hpp>

class SceneNode;

namespace Category {
	enum Type {
		None = 0,
		Scene = 1 << 0,
		PlayerAircraft = 1 << 1,
		AlliedAircraft = 1 << 2,
		EnemyAircraft = 1 << 3,
	};
}

struct Command {
	Command();
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category = 0;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time dt)
	{
		// Check if cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);
		// Downcast node and invoke function on it
		fn(static_cast<GameObject&>(node), dt);
	};
}