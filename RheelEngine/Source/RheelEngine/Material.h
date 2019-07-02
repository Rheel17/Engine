#ifndef MATERIAL_H_
#define MATERIAL_H_
#include "_common.h"

#include "Util/Color.h"

namespace rheel {

class RE_API Material {

public:
	enum MaterialType {
		Colored, Textured
	};

public:
	/**
	 * Creates the default material: a white colored material.
	 */
	Material();

//	/**
//	 * Creates a colored material with the given colors. Additionally, the ambient,
//	 * diffuse, and specular components can be given.
//	 */
//	Material();

private:
	MaterialType _type;
	Color _color;


};

}

#endif
