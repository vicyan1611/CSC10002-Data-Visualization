#pragma once
#include <vector>
#include <memory>
#include <assert.h>
#include <SFML/Graphics.hpp>
#include <CommandQueue.hpp>
#include <Command.hpp>

class SceneNode: public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
public:
	typedef std::unique_ptr<SceneNode> Ptr;
public:
	SceneNode();
	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);
	void update(sf::Time dt);
	sf::Vector2f getWorldPosition() const;
	void onCommand(const Command& command, sf::Time dt);
	virtual unsigned int getCategory() const;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt);
	void updateChildren(sf::Time dt);
	sf::Transform getWorldTransform() const;
private:
	std::vector<Ptr> mChildren;
	SceneNode* mParent;
	
};