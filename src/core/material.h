#pragma once

#include "common.h"

RIGA_NAMESPACE_BEGIN

enum class TransportMode{
	Radiance,
	Important
};

class Material{
public:
	virtual void computeScatteringFunctions(
		SurfaceInteraction* isect, TransportMode mode) const = 0;
	virtual ~Material(){};
};

RIGA_NAMESPACE_END