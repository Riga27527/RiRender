#include "shape.h"

RIGA_NAMESPACE_BEGIN

static long long nShapesCreated = 0;

Shape::Shape(const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient)
			: object2World(obj2wor), world2Object(wor2obj), reverseOrientation(reverseOrient){
				++nShapesCreated;
			}

Shape::~Shape(){}

Bounds3f Shape::worldBound() const{
	return (*object2World)(this->objectBound());
}

RIGA_NAMESPACE_END