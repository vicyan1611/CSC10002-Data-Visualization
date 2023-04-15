#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

namespace Textures {
	enum ID {
		TitleScreen, TitleScreen1, TitleScreen2, TitleScreen3, Eagle, Raptor, Desert,
		ButtonNormal, ButtonSelected, ButtonPressed
	};
}

namespace Fonts {
	enum ID {Main};
}

template <typename Resource, typename Identifier>
class ResourceHolder{
public:
	void load(Identifier id, const std::string& filename);
	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;
private:
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename) {
	std::unique_ptr<Resource> resource(new Resource);
	resource->loadFromFile(filename);
	mResourceMap.insert(std::make_pair(id, std::move(resource)));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) {
	auto f = mResourceMap.find(id);
	return *f->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const {
	auto f = mResourceMap.find(id);
	return *f->second;
}

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;