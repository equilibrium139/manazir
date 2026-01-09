#pragma once

#include "glm/common.hpp"
#include "hittable.h"
#include <cmath>
#include <memory>
#include <numbers>

struct Sphere : public Hittable {
    Ray movement; // Sphere starts at origin and moves in direction defined by ray
    float radius;
    std::shared_ptr<Material> material;

    Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) 
        :movement{.origin = center, .direction = {0.0f, 0.0f, 0.0f}}, radius(radius), material(std::move(material)) {};

    Sphere(const glm::vec3& t0start, const glm::vec3& t0end, float radius, std::shared_ptr<Material> material)
        :movement{.origin = t0start, .direction = t0end - t0start}, radius(radius), material(std::move(material)) {};

    virtual bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const override {
        glm::vec3 sphereCurrentPos = movement.PointAt(ray.time);
        const float a = glm::dot(ray.direction, ray.direction);
        const float b = 2 * glm::dot(ray.direction, ray.origin - sphereCurrentPos);
        const glm::vec3 rayToSphere = sphereCurrentPos - ray.origin;
        const float c = glm::dot(rayToSphere, rayToSphere) - radius * radius;
        const float discriminant = b*b - 4*a*c;
        if (discriminant < 0.0f) return false;

        float sqrtd = std::sqrt(discriminant);
        float root = (-b - sqrtd) / (2.0f*a);
        if (root <= tMin || root >= tMax) {
            root = (-b + sqrtd) / (2.0f*a);
            if (root <= tMin || root >= tMax) {
                return false;
            }
        }
        rec.t = root;
        rec.point = ray.origin + rec.t * ray.direction;
        rec.normal = (rec.point - sphereCurrentPos) / radius;
        rec.material = material;

        if (glm::dot(rec.normal, ray.direction) > 0.0f) {
            rec.normal = -rec.normal;
            rec.frontFace = false;
        }
        else rec.frontFace = true;

        rec.uv = GetUV(rec.normal);

        return true;
    }

    AABB GetAABB() const override {
        // For moving objects, the AABB will be large enough to enclose the object throughout its motion
        glm::vec3 sphereStartPos = movement.origin;
        glm::vec3 sphereEndPos = movement.PointAt(1.0f);
        const glm::vec3 radiusVec = glm::vec3(radius);
        return {
            .minCorner = glm::min(sphereStartPos - radiusVec, sphereEndPos - radiusVec),
            .maxCorner = glm::max(sphereStartPos + radiusVec, sphereEndPos + radiusVec)
        };
    }

    static glm::vec2 GetUV(const glm::vec3& point) {
        float u = (std::atan2(-point.z, point.x) + std::numbers::pi) / (2 * std::numbers::pi);
        float v = std::acos(-point.y) / std::numbers::pi;
        return { u , v };
    }
};
