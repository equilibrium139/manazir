#include <iostream>

#include <glm/glm.hpp>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
    constexpr int imWidth = 200;
    constexpr int imHeight = 200;
    std::vector<glm::u8vec3> pixels(imWidth * imHeight);
    for (int row = 0; row < imHeight; row++) {
        for (int col = 0; col < imWidth; col++) {
            pixels[row*imWidth+col] = { 255, 0, 255 };
        }
    }
    stbi_write_png("manazir.png", imWidth, imHeight, 3, pixels.data(), imWidth * 3);
    return 0;
}
