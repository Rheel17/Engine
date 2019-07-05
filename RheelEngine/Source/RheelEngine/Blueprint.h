#ifndef BLUEPRINT_H_
#define BLUEPRINT_H_
#include "_common.h"

#include <string>
#include <vector>
#include <functional>
#include <map>

#include "Component.h"

namespace rheel {

class RE_API Blueprint {

	using _ComponentLoader = std::function<void(ComponentPtr)>;

public:
	/**
	 * Constructs a blueprint object.
	 */
	Blueprint(std::string name);

	/**
	 * Returns the name of this blueprint.
	 */
	const std::string& Name() { return _name; }

	/**
	 * Adds a component to this blueprint.
	 *
	 * The first argument is the name of the component to add.
	 * The second argument takes a functor with a ComponentPtr as parameter. Use
	 * this function as a (default) initialization of the component when the
	 * blueprint is instantiated. The second argument can be omitted for a no-op
	 * initializer.
	 */
	void AddComponent(const std::string& componentName, _ComponentLoader onLoad = [](ComponentPtr){});

	/**
	 * Adds a nested blueprint. When an instance of this blueprint is created,
	 * all child blueprints are also automatically created.
	 */
	void AddChild(const std::string& blueprintName);

	/**
	 * Returns a vector of all components in this blueprint.
	 */
	const std::vector<std::string> Components() const;

	/**
	 * Returns a vector of all the nested blueprints in this blueprint.
	 */
	const std::vector<std::string> Children() const;

	/**
	 * Returns the component loader for the specified component.
	 */
	const _ComponentLoader& GetLoaderForComponent(const std::string& component) const;

private:
	bool _HasChild(const std::string& name) const;

	std::string _name;

	std::vector<std::string> _components;
	std::map<std::string, _ComponentLoader> _component_loaders;

	std::vector<std::string> _child_blueprints;

};

}

#endif
