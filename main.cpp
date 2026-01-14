#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "camera.h"
#include "hittable.h"
#include "image.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

glm::vec3 PointAt(const Ray& ray, float t) {
    return ray.origin + ray.direction * t;
}

void ManySpheresScene() {
    HittableList world;
    auto groundMaterial = std::make_shared<Lambertian>(std::make_shared<CheckerTexture>(Color(1.0f), Color(0.0f)));
    Add(world, std::make_shared<Sphere>(glm::vec3(0,-1000,0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = RandomFloat();
            glm::vec3 center(a + 0.9*RandomFloat(), 0.2, b + 0.9*RandomFloat());

            if ((center - glm::vec3(4.0f, 0.2f, 0.0f)).length() > 0.9) {
                std::shared_ptr<Material> sphereMaterial;

                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo = RandomVec3() * RandomVec3();
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                    Add(world, std::make_shared<Sphere>(center, center + glm::vec3(0.0f, RandomFloat(0.0f, 0.5f), 0.0f), 0.2f, sphereMaterial));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = RandomVec3(0.5f, 1.0f);
                    auto fuzz = RandomFloat(0.0f, 0.5f);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                    Add(world, std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = std::make_shared<Dielectric>(1.5f);
                    Add(world, std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5f);
    Add(world, make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4f, 0.2f, 0.1f));
    Add(world, make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7f, 0.6f, 0.5f), 0.0f);
    Add(world, make_shared<Sphere>(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, material3));


    Camera camera;
    // camera.cameraPos = glm::vec3(-2.5f, 2.5f, 2.5f);

    camera.Render(world);
}

void TextureMappedSphere() {
    HittableList world;
    auto earthMaterial = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
    Add(world, std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, earthMaterial));

    Camera camera;
    camera.imAspect = 16.0f / 9.0f;
    camera.imWidth = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 50;
    camera.vfov = 20;
    camera.lookAt = glm::vec3(0.0f);
    camera.pos = { 0.0f, 0.0f, 12.0f };
    camera.defocusAngle = 0;
    camera.Render(world);
}

void PerlinSpheres() {
    HittableList world;
    auto perlinTexture = std::make_shared<NoiseTexture>(4.0f);
    Add(world, std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(perlinTexture)));
    Add(world, std::make_shared<Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.0f, std::make_shared<Lambertian>(perlinTexture)));

    Camera camera;
    camera.imAspect = 16.0f / 9.0f;
    camera.imWidth = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 50;
    camera.vfov = 20;
    camera.lookAt = glm::vec3(0.0f);
    camera.pos = { 13.0f, 2.0f, 3.0f };
    camera.defocusAngle = 0;
    camera.Render(world);

}

int world = 2;

int main() {
    // auto ground = std::make_shared<Lambertian>(Color(0.8f, 0.8f, 0.0f));
    // auto center = std::make_shared<Lambertian>(Color(0.1f, 0.2f, 0.5f));
    // auto left = std::make_shared<Dielectric>(1.5f);
    // auto bubble = std::make_shared<Dielectric>(1.0f/1.5f);
    // auto right = std::make_shared<Metal>(Color(0.8f, 0.6f, 0.2f), 1.0f);
    // Add(world, std::make_shared<Sphere>(glm::vec3(0,-100.5f,-1), 100, ground));
    // Add(world, std::make_shared<Sphere>(glm::vec3(0,0,-1.2f), 0.5f, center));
    // Add(world, std::make_shared<Sphere>(glm::vec3(-1,0,-1), 0.5f, left));
    // Add(world, std::make_shared<Sphere>(glm::vec3(-1,0,-1), 0.4f, bubble));
    // Add(world, std::make_shared<Sphere>(glm::vec3(1,0,-1.0f), 0.5f, right));

    // auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    if (world == 0) {
        ManySpheresScene();
    }
    else if (world == 1) {
        TextureMappedSphere();
    }
    else if (world == 2) {
        PerlinSpheres();
    }
    return 0;
}
