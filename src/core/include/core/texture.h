#pragma once

#include <core/common.h>

RIGA_NAMESPACE_BEGIN

template<typename T>
class Texture{
public:
	virtual T evaluate(const SurfaceInteraction& ) const = 0;
	virtual ~Texture(){}
};

RIGA_NAMESPACE_END