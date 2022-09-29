#include "film.h"
#include <fstream>

RIGA_NAMESPACE_BEGIN

void Film::write2PPM(const std::vector<Spectrum>& framebuffer){
	FILE* fp = fopen(fileName.c_str(), "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", fullResolution.x, fullResolution.y);
    for (auto i = 0; i < fullResolution.x * fullResolution.y; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * Clamp(0, 1, framebuffer[i][0]));
        color[1] = (unsigned char)(255 * Clamp(0, 1, framebuffer[i][1]));
        color[2] = (unsigned char)(255 * Clamp(0, 1, framebuffer[i][2]));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp); 
}

RIGA_NAMESPACE_END