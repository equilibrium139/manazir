#pragma once

#include "ray.h"
#include <cfloat>
#include <cmath>
#include <memory>
#include <vector>

struct HitRecord {
    glm::vec3 point;
    glm::vec3 normal;
    float t;
    bool frontFace;
};

class Hittable {
public:
    virtual bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const = 0;
    virtual ~Hittable() = default;
};

using HittableList = std::vector<std::shared_ptr<Hittable>>;

inline void Add(HittableList& list, std::shared_ptr<Hittable> hittable) {
    list.push_back(std::move(hittable));
}

inline bool Hit(const HittableList& list, const Ray& ray, float near, float far, HitRecord& record) {
    float tClosestSoFar = far;
    bool hitSomething = false;
    for (const auto& hittable : list) {
        if (hittable->Hit(ray, near, tClosestSoFar, record)) {
            tClosestSoFar = record.t;
            hitSomething = true;
        }
    }
    return hitSomething;
}
