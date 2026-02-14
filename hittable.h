#pragma once

#include "aabb.h"
#include "ray.h"
#include <cfloat>
#include <cmath>
#include <memory>
#include <span>
#include <vector>

class Material;

struct HitRecord {
    glm::vec3 point;
    glm::vec3 normal;
    std::shared_ptr<Material> material;
    float t;
    glm::vec2 uv;
    bool frontFace;

    void SetFaceNormal(const glm::vec3& rayDir, const glm::vec3& normal) {
        this->normal = normal;
        if (glm::dot(normal, rayDir) > 0.0f) {
            this->normal = -this->normal;
            frontFace = false;
        }
        else frontFace = true;
    }
};

class Hittable {
public:
    virtual bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const = 0;
    virtual AABB GetAABB() const = 0;
    virtual ~Hittable() = default;
    glm::vec3 AABBCenter() const {
        AABB aabb = GetAABB();
        return (aabb.minCorner + aabb.maxCorner) * 0.5f;
    }
};

using HittableList = std::vector<std::shared_ptr<Hittable>>;
using HittableSpan = std::span<std::shared_ptr<Hittable>>;

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

inline AABB HittableSpanAABB(HittableSpan world) {
    AABB aabb = { glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(-std::numeric_limits<float>::infinity()) };
    for (const auto& hittable : world) {
        const AABB hittableAABB = hittable->GetAABB();
        aabb.minCorner = glm::min(hittableAABB.minCorner, aabb.minCorner);
        aabb.maxCorner = glm::max(hittableAABB.maxCorner, aabb.maxCorner);
    }
    return aabb;
};
