#ifndef SCENEELEMENT_H_
#define SCENEELEMENT_H_
#include "../../_common.h"
#include "../../Renderer/SceneRenderManager.h"

#include "../Element.h"
#include "../../Scene.h"

namespace rheel::ui {

class RE_API SceneElement : public Element {

public:
	std::pair<unsigned, unsigned> GetDefaultDimensions() const override;
	void Draw() const override;

private:
	SceneElement(std::string cameraName);
	SceneElement(Scene *scene, std::string cameraName);

	void _InitializeRenderer(const Bounds& bounds) const;

	bool _use_active_scene;
	mutable Scene *_scene;
	std::string _camera_name;

	mutable std::shared_ptr<SceneRenderer> _scene_renderer;

public:
	/**
	 * Creates a UI element that displays the camera output of the
	 * currently active scene. When the currently active scene switches,
	 * the camera switches to that scene automatically, looking for a
	 * camera with the given name.
	 */
	inline static std::shared_ptr<SceneElement> Create(std::string cameraName) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<SceneElement>(new SceneElement(std::move(cameraName)));
	}

	/**
	 * Creates a UI element that displays the camera output of the
	 * given scene. This does not automatically switch when the
	 * current active scene switches.
	 */
	inline static std::shared_ptr<SceneElement> Create(Scene *scene, std::string cameraName) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<SceneElement>(new SceneElement(scene, std::move(cameraName)));
	}

};

}

#endif
