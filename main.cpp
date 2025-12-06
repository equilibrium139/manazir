#include <cmath>
#include <iostream>
#include <vector>

#include "hittable.h"
#include "ray.h"
#include "sphere.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using Color = glm::vec3;

glm::vec3 PointAt(const Ray& ray, float t) {
    return ray.origin + ray.direction * t;
}

// returns <0 for no intersect, >=0 otherwise
Color ComputeRayColor(const Ray& ray, const HittableList& world) {
    static HitRecord record;
    if (Hit(world, ray, 0.0f, INFINITY, record)) {
        glm::vec3 color = record.normal * glm::vec3(0.5f) + glm::vec3(0.5f);
        return color;
    }
    else { 
        constexpr glm::vec3 blue = { 0.5f, 0.7f, 1.0f };
        constexpr glm::vec3 white = { 1.0f, 1.0f, 1.0f };
        const glm::vec3 rayDirNormalized = glm::normalize(ray.direction);
        const float yNormalized = rayDirNormalized.y * 0.5f + 0.5f;
        const float t = yNormalized;
        return white + (blue - white) * t;
    }
}

int main() {
    constexpr int imWidth = 900;
    constexpr float imAspect = 16.0f / 9.0f;
    constexpr int imHeight = imWidth / imAspect;
    constexpr float viewportHeight = 2.0f;
    constexpr float viewportWidth = viewportHeight * ((float)imWidth / imHeight);
    constexpr float focalDistance = 1.0f;
    constexpr float pixelWidth = viewportWidth / imWidth;
    constexpr float pixelHeight = viewportHeight / imHeight;
    constexpr glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f};
    constexpr glm::vec3 cameraDir = {0.0f, 0.0f, -1.0f};
    constexpr glm::vec3 viewportCenter = cameraPos + cameraDir * focalDistance;
    constexpr glm::vec3 viewportTopLeft = viewportCenter - glm::vec3(viewportWidth / 2.0f, -viewportHeight / 2.0f, 0.0f);
    constexpr glm::vec3 topLeftPixel = viewportTopLeft + glm::vec3(pixelWidth / 2.0f, -pixelHeight / 2.0f, 0.0f);
    std::cout << "Viewport center: " << viewportCenter.x << ", " << viewportCenter.y << ", " << viewportCenter.z << '\n';
    std::cout << "Viewport top left: " << viewportTopLeft.x << ", " << viewportTopLeft.y << ", " << viewportTopLeft.z << '\n'; 

    HittableList world;
    Add(world, std::make_shared<Sphere>(glm::vec3(0,0,-1), 0.5));
    Add(world, std::make_shared<Sphere>(glm::vec3(0,-100.5,-1), 100));

    std::vector<glm::u8vec3> pixels(imWidth * imHeight);
    for (int row = 0; row < imHeight; row++) {
        glm::vec3 viewportPoint = topLeftPixel;
        viewportPoint.y -= row * pixelHeight;
        for (int col = 0; col < imWidth; col++) {
            viewportPoint.x += pixelWidth;
            Ray ray = { .origin = cameraPos, .direction = viewportPoint - cameraPos };
            pixels[row*imWidth+col] = ComputeRayColor(ray, world) * 255.999f;
        }
    }
    stbi_write_png("manazir.png", imWidth, imHeight, 3, pixels.data(), imWidth * 3);
    return 0;
}
