#pragma once

#include "hittable.h"
#include <memory>

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

        return true;
    }
};
