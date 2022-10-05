#pragma once

#include "texture.h"

RIGA_NAMESPACE_BEGIN

template<typename T>
class ConstantTexture : public Texture<T>{
public:
	ConstantTexture(const T& v) : value(v){}
	T evaluate(const SurfaceInteraction& ) const {	return value;	}

private:
	T value;
};

RIGA_NAMESPACE_END