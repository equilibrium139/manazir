#pragma once

#include "aabb.h"
#include "hittable.h"
#include "utilities.h"
#include <memory>

class BVH : public Hittable {
public:
    BVH(HittableList world) {
        aabb = HittableListAABB(world);

        int randomAxis = RandomInt(0, 2);
        SortByAxis(world, randomAxis);
    }

    bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const override {
        if (!aabb.Hit(ray, tMin, tMax)) return false;

        return left->Hit(ray, tMin, tMax, rec) || right->Hit(ray, tMin, tMax, rec);
    }

private:
    void SortByAxis(HittableList& world, int axis) {
        if (axis == 0) {
            std::sort(world.begin(), world.end(), [](const auto& a, const auto& b) { return a->AABBCenter().x < b->AABBCenter().x; });
        }
        else if (axis == 1) {
            std::sort(world.begin(), world.end(), [](const auto& a, const auto& b) { return a->AABBCenter().y < b->AABBCenter().y; });
        }
        else {
            std::sort(world.begin(), world.end(), [](const auto& a, const auto& b) { return a->AABBCenter().z < b->AABBCenter().z; });
        }
    }
    
    AABB aabb;
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
};
