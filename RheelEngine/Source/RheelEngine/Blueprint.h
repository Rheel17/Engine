#ifndef BLUEPRINT_H_
#define BLUEPRINT_H_
#include "_common.h"

#include <vector>
#include <memory>

#include "Component.h"

namespace rheel {

class RE_API Blueprint {
	friend class Object;

public:
	/**
	 * Constructs a blueprint object.
	 */
	explicit Blueprint(std::string name);

	/**
	 * Creates a deep copy of this blueprint.
	 */
	Blueprint(const Blueprint& blueprint);

	/**
	 * Returns the name of this blueprint.
	 */
	const std::string& Name() const;

	/**
	 * Adds a component to this blueprint.
	 *
	 * Call this method with the component as template argument like
	 *
	 * auto& component = blueprint.AddComponent<ComponentInstance>();
	 *
	 * The resulting reference can be used to set component parameters. This
	 * reference will be copied into objects instantiated by this blueprint.
	 */
	template<typename T>
	T& AddComponent() {
		static_assert(std::is_base_of<Component, T>::value, "Class is not derived from Component");
		static_assert(std::is_default_constructible<T>::value, "Components must be Default-Constructable");

		std::unique_ptr<T> ptr = std::make_unique<T>();
		T& ref = *ptr;

		_components.push_back(std::move(ptr));
		return ref;
	}

	/**
	 * Returns a pointer to the component of the given type. A nullptr is
	 * returned if no such component exists in this object.
	 */
	template<typename T>
	T *GetComponent() {
		static_assert(std::is_base_of<Component, T>::value, "Type must be a component");

		for (const auto& component : _components) {
			if (auto ptr = dynamic_cast<T *>(component.get())) {
				return ptr;
			}
		}

		return nullptr;
	}

	/**
	 * Adds a nested blueprint. When an instance of this blueprint is created,
	 * all child blueprints are also automatically created.
	 */
	void AddChild(const std::string& blueprintName);

private:
	const std::vector<std::unique_ptr<Component>>& _Components() const;
	const std::vector<std::string>& _Children() const;

	bool _HasChild(const std::string& name) const;

	std::string _name;

	std::vector<std::unique_ptr<Component>> _components;
	std::vector<std::string> _child_blueprints;

};

}

#endif
