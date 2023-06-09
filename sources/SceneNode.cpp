#include <SceneNode.hpp>

SceneNode::SceneNode()
	: mChildren()
	, mParent(nullptr)
{
}

void SceneNode::attachChild(Ptr child) {
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node) {
	auto found = std::find_if(mChildren.begin(), mChildren.end(),
		[&](Ptr& p) -> bool { return p.get() == &node; });
	assert(found != mChildren.end());
	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	drawCurrent(target, states);
	for (const Ptr& child : mChildren) {
		child->draw(target, states);
	}
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
}

void SceneNode::update(sf::Time dt, sf::Time& at) {
	updateCurrent(dt, at);
	updateChildren(dt, at);
}

void SceneNode::updateCurrent(sf::Time dt, sf::Time& at) { 
}

void SceneNode::updateChildren(sf::Time dt, sf::Time& at) {
	for (Ptr& child : mChildren) {
		child->update(dt, at);
	}
}

unsigned int SceneNode::getCategory() const {
	return Category::Scene;
}

void SceneNode::onCommand(const Command& command, sf::Time dt) {
	if (command.category & getCategory()) command.action(*this, dt);
	for (Ptr& child : mChildren) {
		child->onCommand(command, dt);
	}
}