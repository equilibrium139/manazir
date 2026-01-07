#pragma once

#include "stb_image.h"
#include "utilities.h"
#include <cassert>
#include <iostream>

struct Image {
    Image(const char* imgFile) {
        pixels = (Color*)stbi_loadf(imgFile, &width, &height, &channels, 3);
        if (pixels) {
            std::cout << "I successfully loaded the image\n";
        }
    }
    Color PixelAt(int x, int y) const {
        assert((y*width+x) < width*height);
        return pixels[y*width+x];
    }
    Color* pixels;
    int width, height, channels;
};
