#include <cmath>
#include <iostream>
#include <vector>

#include "camera.h"
#include "hittable.h"
#include "ray.h"
#include "sphere.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using Color = glm::vec3;

glm::vec3 PointAt(const Ray& ray, float t) {
    return ray.origin + ray.direction * t;
}

int main() {
    HittableList world;
    Add(world, std::make_shared<Sphere>(glm::vec3(0,0,-1), 0.5));
    Add(world, std::make_shared<Sphere>(glm::vec3(0,-100.5,-1), 100));

    Camera camera;

    camera.Render(world);
    return 0;
}
