#include "glm/geometric.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using Color = glm::u8vec3;

struct Ray {
    glm::vec3 origin, direction;
};

struct Sphere {
    glm::vec3 center;
    float radius;
};

bool RayIntersectsSphere(const Ray& ray, const Sphere& sphere) {
    const float a = glm::dot(ray.direction, ray.direction);
    const float b = 2 * glm::dot(ray.direction, ray.origin - sphere.center);
    const glm::vec3 rayToSphere = sphere.center - ray.origin;
    const float c = glm::dot(rayToSphere, rayToSphere) - sphere.radius * sphere.radius;
    const float determinant = b*b - 4*a*c;
    return determinant >= 0;
}

Color ComputeRayColor(const Ray& ray) {
    static Sphere s = { .center = glm::vec3(0.0f, 0.0f, 3.0f), .radius = 1.0f };
    if (RayIntersectsSphere(ray, s)) {
        return Color(255, 255, 0);
    }
    else return Color(0, 255, 255);
}

int main() {
    constexpr int imWidth = 200;
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

    std::vector<glm::u8vec3> pixels(imWidth * imHeight);
    for (int row = 0; row < imHeight; row++) {
        glm::vec3 viewportPoint = topLeftPixel;
        viewportPoint.y -= row * pixelHeight;
        for (int col = 0; col < imWidth; col++) {
            viewportPoint.x += pixelWidth;
            Ray ray = { .origin = cameraPos, .direction = viewportPoint - cameraPos };
            pixels[row*imWidth+col] = ComputeRayColor(ray);
        }
    }
    stbi_write_png("manazir.png", imWidth, imHeight, 3, pixels.data(), imWidth * 3);
    return 0;
}
