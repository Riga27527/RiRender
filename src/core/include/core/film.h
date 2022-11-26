#pragma once

#include <core/common.h>
#include <core/geometry.h>
#include <core/spectrum.h>
#include <string>
#include <vector>

RIGA_NAMESPACE_BEGIN

class Film{
public:
	Film(const Point2i& resolution, const std::string& file_name = "utitiled.ppm")
	: fullResolution(resolution), fileName(file_name){}

	void write2PPM(const std::vector<Spectrum>& framebuffer);

	const Point2i fullResolution;
	const std::string fileName;
};

RIGA_NAMESPACE_END