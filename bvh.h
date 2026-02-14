#pragma once

#include "aabb.h"
#include "hittable.h"
#include "utilities.h"
#include <memory>

class BVH : public Hittable {
public:
    explicit BVH(HittableList world) : BVH((HittableSpan)world) {}

    explicit BVH(HittableSpan span)
        :aabb(HittableSpanAABB(span)) {
       int countObjects = span.size();
       if (countObjects == 2) {
            left = span[0];
            right = span[1];
            return;
        }
        if (countObjects == 1) {
            // this way we don't need to check for nullptr when calling hit on right or left. they're never nullptr
            left = right = span[0];
            return;
        }
        int longestAxis = aabb.LongestAxis();
        SortByAxis(span, longestAxis);
        int firstHalfEnd = countObjects / 2;
        HittableSpan firstHalf {span.begin(), span.begin() + firstHalfEnd};
        HittableSpan secondHalf {span.begin() + firstHalfEnd, span.end()};
        left = std::make_shared<BVH>(firstHalf);
        right = std::make_shared<BVH>(secondHalf);
    }

    bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const override {
        if (!aabb.Hit(ray, tMin, tMax)) return false;

        bool hitLeft = left->Hit(ray, tMin, tMax, rec);
        bool hitRight = right->Hit(ray, tMin, hitLeft ? rec.t : tMax, rec);
        return hitLeft || hitRight;
    }

    AABB GetAABB() const override {
        return aabb;
    }

private:
    static void SortByAxis(HittableSpan world, int axis) {
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
