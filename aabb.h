#pragma once
#include <glm/glm.hpp>

#include "hittable.h"

struct AABB {
    glm::vec3 minCorner, maxCorner;

    bool Hit(const Ray &ray, const float tMin, const float tMax) const {
        float xDirInverse = 1.0f / ray.direction.x;
        float t0x = (minCorner.x - ray.origin.x) * xDirInverse;
        float t1x = (maxCorner.x - ray.origin.x) * xDirInverse;
        if (t0x > t1x) std::swap(t0x, t1x);
        if (std::isnan(t0x) || std::isnan(t1x)) return false;

        float yDirInverse = 1.0f / ray.direction.y;
        float t0y = (minCorner.y - ray.origin.y) * yDirInverse;
        float t1y = (maxCorner.y - ray.origin.y) * yDirInverse;
        if (t0y > t1y) std::swap(t0y, t1y);
        if (std::isnan(t0y) || std::isnan(t1y)) return false;

        float zDirInverse = 1.0f / ray.direction.z;
        float t0z = (minCorner.z - ray.origin.z) * zDirInverse;
        float t1z = (maxCorner.z - ray.origin.z) * zDirInverse;
        if (t0z > t1z) std::swap(t0z, t1z);
        if (std::isnan(t0z) || std::isnan(t1z)) return false;

        // I really should have implemented the interval class... oh well
        float t0Overlap, t1Overlap;
        if (t0x > t0y) {
            if (t0x >= t1y) return false; // no overlap
            t0Overlap = t0x;
            t1Overlap = std::min(t1x, t1y);
        }
        else {
            if (t0y >= t1x) return false;
            t0Overlap = t0y;
            t1Overlap = std::min(t1x, t1y);
        }
        if (t0Overlap > t0z) {
            if (t0Overlap >= t1z) return false;
            t1Overlap = std::min(t1Overlap, t1z);
        }
        else {
            if (t0z >= t1Overlap) return false;
            t0Overlap = t0z;
            t1Overlap = std::min(t1Overlap, t1z);
        }

        const bool inTRange = t0Overlap > tMin ? (t0Overlap < tMax) : (tMin < t1Overlap);
        return inTRange;
    }
};
