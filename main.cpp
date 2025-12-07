#include <cmath>
#include <iostream>
#include <vector>

#include "camera.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

glm::vec3 PointAt(const Ray& ray, float t) {
    return ray.origin + ray.direction * t;
}

int main() {
    HittableList world;
    auto ground = std::make_shared<Lambertian>(Color(0.8f, 0.8f, 0.0f));
    auto center = std::make_shared<Lambertian>(Color(0.1f, 0.2f, 0.5f));
    auto left = std::make_shared<Dielectric>(1.5f);
    auto bubble = std::make_shared<Dielectric>(1.0f/1.5f);
    auto right = std::make_shared<Metal>(Color(0.8f, 0.6f, 0.2f), 1.0f);
    Add(world, std::make_shared<Sphere>(glm::vec3(0,-100.5f,-1), 100, ground));
    Add(world, std::make_shared<Sphere>(glm::vec3(0,0,-1.2f), 0.5f, center));
    Add(world, std::make_shared<Sphere>(glm::vec3(-1,0,-1), 0.5f, left));
    Add(world, std::make_shared<Sphere>(glm::vec3(-1,0,-1), 0.4f, bubble));
    Add(world, std::make_shared<Sphere>(glm::vec3(1,0,-1.0f), 0.5f, right));

    Camera camera;

    camera.Render(world);
    return 0;
}
